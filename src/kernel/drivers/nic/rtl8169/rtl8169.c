/*
    @JakubPrzystasz
    Created: 06.02.2021
    Last modified: 20.02.2023
*/
#include "rtl8169.h"

static const char RTL8169_NAME[] = "RTL8169";

static uint8_t devices_count = 0;
static rtl8169_dev_t *devices;

static uint32_t DESC_COUNT = 32;

uint32_t rtl8169_init(net_device_t *(*get_net_device)())
{
    // First get the network device using PCI
    for (uint8_t i = 0; i < pci_get_number_of_devices(); i++)
    {
        pci_device *pci_dev = pci_get_device(i);
        if (pci_dev->config.vendor_id == 0x10ec && pci_dev->config.device_id == 0x8136)
        {
            devices = heap_kernel_realloc(devices, sizeof(rtl8169_dev_t) * ++devices_count, 0);
            rtl8169_dev_t *dev = &devices[devices_count - 1];
            memset(dev, 0x0, sizeof(rtl8169_dev_t));

            dev->pci = pci_dev;
            dev->dev->priv = dev;
            dev->dev = get_net_device();

            // Enable IRQ
            dev->irq_vector = dev->pci->config.interrupt_line;
            idt_attach_interrupt_handler(dev->irq_vector, rtl8169_irq_handler);
            pic_enable_irq(dev->irq_vector);

            // Bus mastering
            pci_busmaster_set(dev->pci, true);

            // Now setup registers, and memory
            dev->io_type = dev->pci->config.base_addres_0 & (0x1);
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
            memcpy(&dev->dev->interface.name, name,strlen(name));
            dev->dev->dpi.send = &rtl8169_send;

            // Software reset
            rtl8169_write_byte(dev, CR, (rtl8169_cr_t){.RST = 1}.value);
            while (((rtl8169_cr_t)rtl8169_read_byte(dev, CR)).RST)
                ;
            logger_log_info("SW RESET");

            // Set device into config mode:
            // rtl8169_write_byte(dev, CR9346, (rtl8169_9346cr_t){.EEM = RTL8169_CONFIG}.value);
            rtl8169_write_byte(dev, CR9346, 0xC0);
            logger_log_info("CFG MODE");

            rtl8169_write_byte(dev, CR, (rtl8169_cr_t){.RE = 1, .TE = 1}.value);
            logger_log_info("RX TX EN");

            // rtl8169_write_word(dev, IMR, (rtl8169_imr_t){
            //                                  .TOK = 1,
            //                                  .ROK = 1,
            //                              }
            //                                  .value);
            rtl8169_write_word(dev, IMR, 0xFFFF);
            logger_log_info("IMR");

            rtl8169_write_byte(dev, ETThR, (rtl8169_etthr_t){.ETTh = 0b11000}.value);
            rtl8169_write_byte(dev, RMS, (rtl8169_rms_t){.RMS = 1520}.value);

            // Setup receive descriptors:
            dev->rx_ptr = 0;
            dev->rx = heap_kernel_alloc(sizeof(rtl8169_rx_descriptor_t) * DESC_COUNT, 0);
            for (uint32_t i = 0; i < DESC_COUNT; i++)
            {
                rtl8169_rx_descriptor_t *desc = dev->rx + i;
                desc->command = (0x80000000 | (1520 & 0x3FFF));
                desc->vlan = 0;
                desc->high_address = 0x0;
                desc->low_address = heap_kernel_alloc(1520, 0) - DMA_ADDRESS_OFFSET;
                if ((i + 2) >= DESC_COUNT)
                    desc->command = (0x80000000 | 0x40000000 | (1520 & 0x3FFF));
            }
            rtl8169_write_long(dev, RDSA_HIGH, 0x0);
            rtl8169_write_long(dev, RDSA_LOW, dev->rx - DMA_ADDRESS_OFFSET);

            // Setup transmitt descriptors
            dev->tx_ptr = 0;
            dev->tx = heap_kernel_alloc(sizeof(rtl8169_tx_descriptor_t) * DESC_COUNT, 0);
            for (uint32_t i = 0; i < DESC_COUNT; i++)
            {
                rtl8169_tx_descriptor_t *desc = dev->tx + i;
                desc->OWN = 0;
                desc->high_address = 0x0;
                desc->length = 1520;
                desc->low_address = heap_kernel_alloc(1520, 0) - DMA_ADDRESS_OFFSET;
                if ((i + 2) >= DESC_COUNT)
                    desc->EOR = 1;
            }
            rtl8169_write_long(dev, TNPDS_HIGH, 0x0);
            rtl8169_write_long(dev, TNPDS_LOW, dev->tx - DMA_ADDRESS_OFFSET);

            logger_log_info("DESCRIPTORS INIT DONE");

            rtl8169_write_long(dev, RCR, (rtl8169_rxcfg_t){.APM = 1, .AM = 1, .AB = 1, .MXDMA = 0b111, .RXFTH = 0b111}.value);
            logger_log_info("RECEIVE CFG");

            // Set driver load
            rtl8169_config1_t config1 = (rtl8169_config1_t)rtl8169_read_byte(dev, CONFIG1);
            config1.DVRLOAD = 1;
            rtl8169_write_byte(dev, CONFIG1, config1.value);
            rtl8169_write_byte(dev, CR9346, (rtl8169_9346cr_t){.EEM = RTL8169_NORMAL}.value);
            logger_log_info("DRIVER INIT DONE");
        }
    }
    if (devices_count)
        return true;
    return false;
}

bool rtl8169_irq_handler(void)
{
    // Get status of device
    for (uint32_t dev_id = 0; dev_id < devices_count; dev_id++)
    {
        rtl8169_dev_t *dev = &devices[dev_id];
        dev->isr = (rtl8169_isr_t)rtl8169_read_word(dev, ISR);

        if (dev->isr.value == 0)
            continue;

        char tmp[64];
        kernel_sprintf(tmp, "ISR: 0x%x", dev->isr);
        logger_log_warning(tmp);

        if (dev->isr.TOK)
            ;

        if (dev->isr.ROK)
            rtl8169_receive(dev);

        rtl8169_write_word(dev, ISR, dev->isr.value);

        return true;
    }

    return false;
}

static uint32_t cnt = 0;
uint32_t rtl8169_receive(rtl8169_dev_t *dev)
{

    for (uint32_t i = 0; i < DESC_COUNT; i++)
    {
        rtl8169_tx_descriptor_t *desc = dev->tx + i;
        if (!desc->OWN)
        {
            char tmp[128];
            kernel_sprintf(tmp, "%d %d %d bytes on: %02x:%02x:%02x:%02x:%02x:%02x",
                           i,
                           cnt++,
                           desc->length,
                           dev->dev->interface.mac.octet_a,
                           dev->dev->interface.mac.octet_b,
                           dev->dev->interface.mac.octet_c,
                           dev->dev->interface.mac.octet_d,
                           dev->dev->interface.mac.octet_e,
                           dev->dev->interface.mac.octet_f);
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
        device->dev->interface.mac.addr[octet] = rtl8169_read_byte(device, IDR0 + octet);
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
