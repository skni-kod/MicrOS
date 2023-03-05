/*
    @JakubPrzystasz
    Created: 06.02.2021
    Last modified: 20.02.2023
*/
#include "rtl8139.h"
#include "rtl8139_regs.h"

static const rtl8139_hw_ver_t const rtl8139_hw_versions[] = {
    (rtl8139_hw_ver_t){.value = 0b110000, .string = "RTL8139"},
    (rtl8139_hw_ver_t){.value = 0b111000, .string = "RTL8139A"},
    (rtl8139_hw_ver_t){.value = 0b111001, .string = "RTL8139A-G"},
    (rtl8139_hw_ver_t){.value = 0b111100, .string = "RTL8139B"},
    (rtl8139_hw_ver_t){.value = 0b111110, .string = "RTL8130"},
    (rtl8139_hw_ver_t){.value = 0b111010, .string = "RTL8139C"},
    (rtl8139_hw_ver_t){.value = 0b111011, .string = "RTL8139C+"},
    (rtl8139_hw_ver_t){.value = 0b111111, .string = "RTL NONAME"}};

static uint8_t devices_count = 0;
static void **devices;

static const uint8_t const TSAD_array[] = {0x20, 0x24, 0x28, 0x2C};
static const uint8_t const TSD_array[] = {0x10, 0x14, 0x18, 0x1C};

bool rtl8139_probe(net_device_t *(*get_net_device)())
{
    // First get the network device using PCI
    for (uint8_t i = 0; i < pci_get_number_of_devices(); i++)
    {
        pci_device *pci_dev = pci_get_device(i);
        if (pci_dev->config.vendor_id == RTL8139_VENDOR_ID && pci_dev->config.device_id == RTL8139_DEVICE_ID)
        {
            rtl8139_dev_t *dev = heap_kernel_alloc(sizeof(rtl8139_dev_t), 0);
            devices = heap_kernel_realloc(devices, sizeof(rtl8139_dev_t *) * ++devices_count, 0);
            devices[devices_count - 1] = dev;
            memset(dev, 0x0, sizeof(rtl8139_dev_t));

            dev->pci = pci_dev;
            dev->net = get_net_device();
            dev->net->priv = dev;

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

            // Bus mastering
            pci_busmaster_set(dev->pci, true);

            // Enable IRQ
            dev->irq_vector = dev->pci->config.interrupt_line;
            idt_attach_interrupt_handler(dev->irq_vector, rtl8139_irq_handler);
            pic_enable_irq(dev->irq_vector);

            rtl8139_write_word(dev, MULTINT, 0x0);
            rtl8139_write_word(dev, IMR, 0xFFFF);

            // Setup net device
            rtl8139_read_mac(dev);
            const char *name = rtl8139_get_name(dev);
            memcpy(dev->net->interface.name, name, strlen(name) + 1);

            dev->net->dpi.send = &rtl8139_send;
            dev->rx = rtl8139_init_buffer(RTL8139_RX_BUFFER_BASE);
            if (!dev->rx)
                return false;

            rtl8139_write_word(dev, CAPR, dev->rx->pointer);
            dev->tsad = 0;

            // Software reset
            rtl8139_write_byte(dev, CR, (rtl8139_cr_t){.RST = 1}.value);
            while (((rtl8139_cr_t)rtl8139_read_byte(dev, CR)).RST)
                ;

            rtl8139_write_byte(dev, CR, (rtl8139_cr_t){.RE = 1, .TE = 1}.value);

            rtl8139_write_long(dev, RBSTART, dev->rx->dma_address);

            rtl8139_write_long(dev, RCR, 0xf | (1 << 7));

            // Set device into config mode:
            rtl8139_write_byte(dev, CR9346, 0b11);
            // Set driver load
            rtl8139_config1_t config1 = (rtl8139_config1_t)rtl8139_read_byte(dev, CONFIG1);
            config1.DVRLOAD = 1;
            rtl8139_write_byte(dev, CONFIG1, config1.value);
            rtl8139_write_byte(dev, CR9346, 0);
        }
    }
    if (devices_count)
        return true;
    return false;
}

bool rtl8139_irq_handler(void)
{
    logger_log_info("RTL ISR HANDLER");

    // Get status of device
    for (uint32_t dev_id = 0; dev_id < devices_count; dev_id++)
    {
        rtl8139_dev_t *dev = devices[0];
        rtl8139_isr_t status = (rtl8139_isr_t)rtl8139_read_word(dev, ISR);

        if (!status.value)
            continue;

        char tmp[128];
        kernel_sprintf(tmp, "ISR STATUS: 0x%x", status.value);
        logger_log_info(tmp);

        rtl8139_write_word(dev, ISR, status.value);

        // if (status.ROK)
        //     return rtl8139_receive(dev);

        return true;
    }

    return false;
}

uint32_t rtl8139_receive(rtl8139_dev_t *dev)
{
    rtl8139_rx_header *frame = ((uint8_t *)(dev->rx->data) + dev->rx->pointer);
    if (frame->ROK)
    {
        nic_data_t *out = dev->net->dpi.get_receive_buffer(dev->net);
        memcpy(out->frame, frame->data, (uint32_t)frame->size);

        dev->rx->pointer = ((dev->rx->pointer + (uint32_t)frame->size + 4 + 3)) & (~3);
        dev->rx->pointer = dev->rx->pointer % RTL8139_RX_BUFFER_BASE;

        rtl8139_write_word(dev, CAPR, dev->rx->pointer - 0x10);
        (*dev->net->dpi.receive)(out);
    }
}

uint32_t rtl8139_send(nic_data_t *data)
{
    rtl8139_dev_t *dev = data->device->priv;

    uint32_t *snd_data = heap_kernel_alloc(data->length, 4);
    memcpy(snd_data, &data->frame, data->length);

    rtl8139_write_long(dev, TSAD_array[dev->tsad], GET_PHYSICAL_ADDRESS(snd_data));
    rtl8139_write_long(dev, TSD_array[dev->tsad], (rtl8139_tsd_t){.SIZE = data->length, .OWN = 0}.value);

    // Switch buffer
    dev->tsad = (dev->tsad + 1) % 4;
}

void rtl8139_read_mac(rtl8139_dev_t *device)
{
    for (uint8_t octet = 0; octet < sizeof(mac_addr_t); octet++)
        device->net->interface.mac.addr[octet] = rtl8139_read_byte(device, IDR0 + octet);
}

const char *rtl8139_get_name(rtl8139_dev_t *device)
{
    rtl8139_txcfg_t cfg = (rtl8139_txcfg_t)rtl8139_read_long(device, TCR);
    rtl8139_hw_ver_t hw_ver = (rtl8139_hw_ver_t){.hw_ver_a = cfg.HWVERID_A, .hw_ver_b = cfg.HWVERID_B};
    uint8_t hw;

    for (hw = 0; hw < sizeof(rtl8139_hw_versions) / sizeof(rtl8139_hw_ver_t); hw++)
    {
        if (rtl8139_hw_versions[hw].value == hw_ver.value)
            return rtl8139_hw_versions[hw].string;
    }

    return rtl8139_hw_versions[hw].string;
}

void rtl8139_write_byte(rtl8139_dev_t *device, uint32_t address, uint8_t value)
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

uint8_t rtl8139_read_byte(rtl8139_dev_t *device, uint32_t address)
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

void rtl8139_write_word(rtl8139_dev_t *device, uint32_t address, uint16_t value)
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

uint16_t rtl8139_read_word(rtl8139_dev_t *device, uint32_t address)
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

void rtl8139_write_long(rtl8139_dev_t *device, uint32_t address, uint32_t value)
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

uint32_t rtl8139_read_long(rtl8139_dev_t *device, uint32_t address)
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

rtl8139_buffer_t *rtl8139_init_buffer(uint32_t size)
{
    rtl8139_buffer_t *buffer = 0;
    buffer = heap_kernel_alloc(sizeof(rtl8139_buffer_t) + size, 4);
    memset(buffer, 0, sizeof(rtl8139_buffer_t) + size);
    if (buffer)
    {
        buffer->size = size;
        buffer->pointer = 0;
        buffer->dma_address = (uint32_t)buffer->data - (uint32_t)DMA_ADDRESS_OFFSET;
        return buffer;
    }
    else
        return 0;
}
