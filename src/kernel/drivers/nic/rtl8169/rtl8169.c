/*
    @JakubPrzystasz
    Created: 06.02.2021
    Last modified: 20.02.2023
*/
#include "rtl8169.h"
#include "rtl8169_regs.h"

static const char RTL8169_NAME[] = "RTL8169";

static uint8_t devices_count = 0;
static void **devices;

#define DESCRIPTOR_COUNT 32
#define BUFFER_SIZE 1520

bool rtl8169_probe(net_device_t *(*get_net_device)())
{
    // First get the network device using PCI
    for (uint8_t i = 0; i < pci_get_number_of_devices(); i++)
    {
        pci_device *pci_dev = pci_get_device(i);
        if (pci_dev->config.vendor_id == RTL8169_VENDOR_ID)
        {
            switch (pci_dev->config.device_id)
            {
            case 0x8169:
            case 0x8168:
            case 0x8129:
                goto device_found;
            default:
                break;
            };

            continue;

        device_found:

            devices = heap_kernel_realloc(devices, sizeof(rtl8169_dev_t) * ++devices_count, 0);
            rtl8169_dev_t *dev = &devices[devices_count - 1];
            memset(dev, 0x0, sizeof(rtl8169_dev_t));

            dev->pci = pci_dev;
            dev->net = get_net_device();
            dev->net->priv = dev;

            // Bus mastering
            pci_busmaster_set(dev->pci, true);

            // Enable interrupts in CPU
            dev->irq_vector = dev->pci->config.interrupt_line;
            idt_attach_interrupt_handler(dev->irq_vector, rtl8169_irq_handler);
            pic_enable_irq(dev->irq_vector);

            // Now setup registers, and memory
            dev->io_type = (io_type_t)(dev->pci->config.base_addres_0 & (0x1));
            switch (dev->io_type)
            {
            case IO:
                dev->io_base = dev->pci->config.base_addres_0 & (~0x3);
                break;
            case MMAP:
                dev->mem_base = dev->pci->config.base_addres_0 & (~0xf);
                break;
            }

            // Setup net device
            rtl8169_read_mac(dev);
            const char *name = rtl8169_get_name(dev);
            memcpy(&dev->net->interface.name, name, strlen(name));
            dev->net->dpi.send = &rtl8169_send;

            // Software reset
            rtl8169_write_byte(dev, CR, (rtl8169_cr_t){.RST = 1}.value);
            while (((rtl8169_cr_t)rtl8169_read_byte(dev, CR)).RST)
                ;

            // First enable TX and RX
            rtl8169_write_byte(dev, CR, (rtl8169_cr_t){.RE = 1, .TE = 1}.value);
            // Configure RX
            rtl8169_write_long(dev, RCR, (rtl8169_rxcfg_t){
                                             .AAP = 1,
                                             .APM = 1,
                                             .AM = 1,
                                             .AB = 1,
                                             .AR = 1,
                                             .MXDMA = 0b111,
                                             .RXFTH = 0b111,
                                             .MULERTH = 0,
                                             .RER8 = 1,
                                         }
                                             .value);

            rtl8169_write_byte(dev, RMS, (rtl8169_rms_t){.RMS = BUFFER_SIZE}.value);

            // Setup receive descriptors:
            dev->rx_ptr = 0;
            dev->rx = heap_kernel_alloc(sizeof(rtl8169_rx_descriptor_t) * DESCRIPTOR_COUNT, 256);
            for (uint32_t i = 0; i < DESCRIPTOR_COUNT; i++)
            {
                rtl8169_rx_descriptor_t *desc = dev->rx + i;
                desc->OWN = 1;
                desc->EOR = 0;
                desc->length = BUFFER_SIZE;
                desc->tava = 0;
                desc->vlan = 0;
                desc->high_address = 0;
                desc->low_address = heap_kernel_alloc(BUFFER_SIZE, 8) - DMA_ADDRESS_OFFSET;
                if ((i + 1) == DESCRIPTOR_COUNT)
                    desc->EOR = 1;
            }
            rtl8169_write_long(dev, RDSA_HIGH, 0x0);
            rtl8169_write_long(dev, RDSA_LOW, dev->rx - DMA_ADDRESS_OFFSET);

            // Configure TX
            rtl8169_write_byte(dev, ETThR, (rtl8169_etthr_t){.ETTh = 0b11111}.value);
            // Setup transmit descriptors
            dev->tx_ptr = 0;
            dev->tx = heap_kernel_alloc(sizeof(rtl8169_tx_descriptor_t) * DESCRIPTOR_COUNT, 256);
            for (uint32_t i = 0; i < DESCRIPTOR_COUNT; i++)
            {
                rtl8169_rx_descriptor_t *desc = dev->rx + i;
                desc->OWN = 0;
                desc->EOR = 0;
                desc->length = BUFFER_SIZE;
                desc->tava = 0;
                desc->vlan = 0;
                desc->high_address = 0;
                desc->low_address = heap_kernel_alloc(BUFFER_SIZE, 8) - DMA_ADDRESS_OFFSET;
                if ((i + 1) == DESCRIPTOR_COUNT)
                    desc->EOR = 1;
            }
            rtl8169_write_long(dev, TNPDS_HIGH, 0x0);
            rtl8169_write_long(dev, TNPDS_LOW, dev->tx - DMA_ADDRESS_OFFSET);

            rtl8169_write_word(dev, IMR, (rtl8169_imr_t){
                                             .TOK = 1,
                                             .ROK = 1,
                                         }
                                             .value);

            // Set device into config mode:
            {
                rtl8169_9346cr_t reg = (rtl8169_9346cr_t)rtl8169_read_byte(dev, CR9346);
                reg.EEM = RTL8169_CONFIG;
                rtl8139_write_byte(dev, CR9346, reg.value);

                // Set driver load
                rtl8169_config1_t config1 = (rtl8169_config1_t)rtl8169_read_byte(dev, CONFIG1);
                config1.DVRLOAD = 1;
                rtl8139_write_byte(dev, CONFIG1, config1.value);

                reg.EEM = RTL8169_NORMAL;
                rtl8139_write_byte(dev, CR9346, reg.value);
            }
        }
    }

    if (devices_count)
        return true;
    return false;
}

bool rtl8169_irq_handler(interrupt_state *state)
{
    // Get status of device
    for (uint32_t dev_id = 0; dev_id < devices_count; dev_id++)
    {
        rtl8169_dev_t *dev = &devices[dev_id];
        rtl8169_isr_t isr = (rtl8169_isr_t)rtl8169_read_word(dev, ISR);

        if (isr.value == 0)
            continue;

        if (isr.TOK)
            ;

        if (isr.ROK)
            rtl8169_receive(dev);

        rtl8169_write_word(dev, ISR, isr.value);

        return true;
    }

    return false;
}

uint32_t rtl8169_receive(rtl8169_dev_t *dev)
{

    for (uint32_t i = 0; i < DESCRIPTOR_COUNT; i++)
    {
        rtl8169_tx_descriptor_t *desc = dev->tx + i;
        if (!desc->OWN)
        {
            char tmp[128];
            kernel_sprintf(tmp, "%d bytes on: %02x:%02x:%02x:%02x:%02x:%02x",
                           desc->length,
                           dev->net->interface.mac.octet_a,
                           dev->net->interface.mac.octet_b,
                           dev->net->interface.mac.octet_c,
                           dev->net->interface.mac.octet_d,
                           dev->net->interface.mac.octet_e,
                           dev->net->interface.mac.octet_f);
            logger_log_info(tmp);
            desc->OWN = 1;
            // nic_data_t *data = dev->dev->dpi.get_receive_buffer(dev->dev);
            // memcpy(&data->frame, desc->low_address + DMA_ADDRESS_OFFSET, desc->length);
            // data->keep = 0;
            // data->length = desc->length;
            // desc->OWN = 1;
            // (*dev->dev->dpi.receive)(&data);
        }
    }

    // if (!desc->OWN)
    // {

    // }
}

uint32_t rtl8169_send(nic_data_t *data)
{
}

void rtl8169_read_mac(rtl8169_dev_t *device)
{
    for (uint8_t octet = 0; octet < sizeof(mac_addr_t); octet++)
        device->net->interface.mac.addr[octet] = rtl8169_read_byte(device, IDR0 + octet);
}

const char *rtl8169_get_name(rtl8169_dev_t *device)
{
    return RTL8169_NAME;
}

void rtl8169_write_byte(rtl8169_dev_t *device, uint32_t address, uint8_t value)
{
    switch (device->io_type)
    {
    case IO:
        io_out_byte(device->io_base + address, value);
        break;
    case MMAP:
        // TODO:
        break;
    }
}

uint8_t rtl8169_read_byte(rtl8169_dev_t *device, uint32_t address)
{
    switch (device->io_type)
    {
    case IO:
        return io_in_byte(device->io_base + address);
    case MMAP:
        // TODO:
        break;
    }
}

void rtl8169_write_word(rtl8169_dev_t *device, uint32_t address, uint16_t value)
{
    switch (device->io_type)
    {
    case IO:
        io_out_word(device->io_base + address, value);
        break;
    case MMAP:
        // TODO:
        break;
    }
}

uint16_t rtl8169_read_word(rtl8169_dev_t *device, uint32_t address)
{
    switch (device->io_type)
    {
    case IO:
        return io_in_word(device->io_base + address);
    case MMAP:
        // TODO:
        break;
    }
}

void rtl8169_write_long(rtl8169_dev_t *device, uint32_t address, uint32_t value)
{
    switch (device->io_type)
    {
    case IO:
        io_out_long(device->io_base + address, value);
        break;
    case MMAP:
        // TODO:
        break;
    }
}

uint32_t rtl8169_read_long(rtl8169_dev_t *device, uint32_t address)
{
    switch (device->io_type)
    {
    case IO:
        return io_in_long(device->io_base + address);
    case MMAP:
        // TODO:
        break;
    }
}
