/*
    @JakubPrzystasz
    Created: 06.02.2021
    Last modified: 20.02.2023
*/
#include "rtl8169.h"
#include "rtl8169_regs.h"

#define ALIGN(x, a) __ALIGN_MASK(x, (__typeof__(x))(a)-1)
#define __ALIGN_MASK(x, mask) (((x) + (mask)) & ~(mask))

#define DESCRIPTOR_COUNT 16
#define BUFFER_SIZE 8191

#define DESCRIPTOR_ALIGN 256
#define BUFFER_ALIGN 8

static const char RTL8169_NAME[] = "RTL8169";

static uint8_t devices_count = 0;
static void **devices;

uint8_t rx_plane[1024 * 1024];
uint8_t tx_plane[1024 * 1024];

char str[128];

void rtl8169_print_descriptor(int id, rtl8169_rx_descriptor_t *desc)
{
    kernel_sprintf(str, "I: %d O:%d E:%d L: %d B:0x%x 0x%x",
                   id,
                   desc->OWN,
                   desc->EOR,
                   desc->length,
                   desc->low_address,
                   desc->command_long);
    logger_log_info(str);
}

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
                goto found;
            default:
                continue;
            };
        found:
            devices = heap_kernel_realloc(devices, sizeof(rtl8169_dev_t *) * ++devices_count, 0);
            rtl8169_dev_t *dev = heap_kernel_alloc(sizeof(rtl8169_dev_t), 0);
            devices[devices_count - 1] = dev;
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

            // Setup receive descriptors:
            // dev->rx = (void *)ALIGN((uint32_t)heap_kernel_alloc(sizeof(rtl8169_rx_descriptor_t) * DESCRIPTOR_COUNT, 16),16);
            dev->rx = (void *)ALIGN((uint32_t)rx_plane, 16);
            dev->rx_ptr = 0;
            for (uint32_t i = 0; i < DESCRIPTOR_COUNT; i++)
            {
                rtl8169_rx_descriptor_t *desc = dev->rx + i;
                desc->OWN = 1;
                desc->EOR = 0;
                desc->length = BUFFER_SIZE;
                desc->vlan_long = 0;
                desc->high_address = 0;
                desc->low_address = (uint32_t)heap_kernel_alloc(BUFFER_SIZE, BUFFER_ALIGN) - DMA_ADDRESS_OFFSET;
            }
            {
                rtl8169_rx_descriptor_t *desc = dev->rx + (DESCRIPTOR_COUNT - 1);
                desc->EOR = 1;
            }

            // Setup transmit descriptors
            dev->tx_ptr = 0;
            dev->tx = (void *)ALIGN((uint32_t)tx_plane, 16);
            for (uint32_t i = 0; i < DESCRIPTOR_COUNT; i++)
            {
                rtl8169_tx_descriptor_t *desc = dev->tx + i;
                desc->command_long = 0;
                desc->OWN = 0;
                desc->EOR = 0;
                desc->LGSEN = 0;
                desc->length = BUFFER_SIZE;
                desc->vlan_long = 0;
                desc->high_address = 0;
                desc->low_address = (uint32_t)heap_kernel_alloc(BUFFER_SIZE, BUFFER_ALIGN) - DMA_ADDRESS_OFFSET;
            }
            {
                rtl8169_tx_descriptor_t *desc = dev->tx + (DESCRIPTOR_COUNT - 1);
                desc->EOR = 1;
            }

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
            const char *name = rtl8169_get_name(dev);
            memcpy(&dev->net->interface.name, name, strlen(name));
            dev->net->dpi.send = &rtl8169_send;

            // Software reset
            rtl8169_write_byte(dev, CR, (rtl8169_cr_t){.RST = 1}.value);
            while (((rtl8169_cr_t)rtl8169_read_byte(dev, CR)).RST)
                ;

            // Set device into config mode:
            rtl8169_9346cr_t reg = (rtl8169_9346cr_t)rtl8169_read_byte(dev, CR9346);
            reg.EEM = RTL8169_CONFIG;
            rtl8139_write_byte(dev, CR9346, reg.value);

            // Get device MAC
            rtl8169_read_mac(dev);

            // Enable TX to allow configuration of TX
            rtl8169_write_byte(dev, CR, (rtl8169_cr_t){.TE = 1}.value);

            // Configure TX
            {
                rtl8169_write_byte(dev, ETThR, ((rtl8169_etthr_t){.ETTh = 0b11111}).value);
                rtl8169_write_long(dev, TNPDS_HIGH, 0x0);
                rtl8169_write_long(dev, TNPDS_LOW, (uint32_t)(dev->tx) - (uint32_t)DMA_ADDRESS_OFFSET);
            }

            // Configure RX
            {
                rtl8169_write_long(dev, RDSA_HIGH, 0x0);
                rtl8169_write_long(dev, RDSA_LOW, (uint32_t)(dev->rx) - (uint32_t)DMA_ADDRESS_OFFSET);
                rtl8169_write_word(dev, RMS, ((rtl8169_rms_t){.RMS = BUFFER_SIZE}).value);
                rtl8169_write_long(dev, RCR, (rtl8169_rxcfg_t){
                                                 .AAP = 1,
                                                 .APM = 1,
                                                 .AM = 1,
                                                 .AB = 1,
                                                 .AR = 1,
                                                 .MXDMA = 0b111,
                                                 .RXFTH = 0b111,
                                                 .MULERTH = 0,
                                                 .RER8 = 0,
                                             }
                                                 .value);
            }

            // Set interrupts
            rtl8169_write_word(dev, IMR, 0xff);

            // Set driver load
            rtl8169_config1_t config1 = (rtl8169_config1_t)rtl8169_read_byte(dev, CONFIG1);
            config1.DVRLOAD = 1;
            rtl8139_write_byte(dev, CONFIG1, config1.value);

            // Enable RX and TX
            rtl8169_write_byte(dev, CR, ((rtl8169_cr_t){.TE = 1, .RE = 1}).value);

            // Back to normal operation
            reg.EEM = RTL8169_NORMAL;
            rtl8139_write_byte(dev, CR9346, reg.value);
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
        rtl8169_dev_t *dev = devices[dev_id];
        rtl8169_isr_t isr = (rtl8169_isr_t)rtl8169_read_word(dev, ISR);

        if (!isr.value)
            continue;

        rtl8169_write_word(dev, ISR, isr.value);

        kernel_sprintf(str, "ISR: 0x%x", isr.value);
        logger_log_info(str);

        if (isr.ROK)
            rtl8169_receive(dev);

        return true;
    }

    return false;
}

uint32_t rtl8169_receive(rtl8169_dev_t *dev)
{
    rtl8169_rx_descriptor_t *desc = dev->rx + dev->rx_ptr;
    if (desc->OWN == 0)
    {
        nic_data_t *data = dev->net->dpi.get_receive_buffer(dev->net);
        memcpy(data->frame, (uint32_t)desc->low_address + (uint32_t)DMA_ADDRESS_OFFSET, desc->length);
        data->keep = 0;
        data->length = desc->length;

        dev->rx_ptr++;
        if (desc->EOR)
            dev->rx_ptr = 0;

        (*dev->net->dpi.receive)(data);
        desc->OWN = 1;
        desc->length = BUFFER_SIZE;
    }
}

uint32_t rtl8169_send(nic_data_t *data)
{
    rtl8169_dev_t *dev = (rtl8169_dev_t *)data->device->priv;

    rtl8169_tx_descriptor_t *desc = dev->tx + dev->tx_ptr;
    desc->length = data->length;
    memcpy((uint32_t)desc->low_address + (uint32_t)DMA_ADDRESS_OFFSET, data->frame, data->length);
    dev->tx_ptr++;
    if (desc->EOR)
        dev->tx_ptr = 0;
    // Send data!
    desc->OWN = 1;
    // rtl8139_write_byte(dev, TPPoll, (rtl8169_tppoll_t){.NPQ = 1}.value);
    rtl8139_write_byte(dev, TPPoll, 0x40);
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
