/*
    @JakubPrzystasz
    Created: 06.02.2021
    Last modified: 17.03.2022
*/
#include "rtl8139.h"
#include "rtl8139_hw_regs.h"

static pci_device pci_rtl8139_device;
static rtl8139_dev_t rtl8139_device;
static net_device_t *rtl8139_net_device;

static char device_name[] = RTL8139_DEVICE_NAME;

// FIXME: add this to macro
uint32_t TX_MAX_DESC = 4;
uint32_t R8139DN_TX_DESC_SIZE = 1792;

uint8_t rx_data[10 * 1600];

int rtl8139_reset(rtl8139_dev_t *dev)
{
    int i = 1000;

    // Ask the chip to reset
    rtl_write_byte(CR, CR_RST);

    // Wait until the reset is complete or timeout
    // The chip notify us by clearing the bit
    while (--i)
    {
        if (!(rtl_read_byte(CR) & CR_RST))
        {
            // Reset is complete!
            break;
        }
        timer_sleep(1);
    }

    if (!i)
        return -1;

    // Resetting the chip also resets hardware TX pointer to TSAD0
    // So we need to keep track of this, and we also reset our own position
    dev->rx->cur = 0;
    dev->tx->cur = 0;

    return 0;
}

void rtl8139_setup_tx(rtl8139_dev_t *dev)
{
    int i;
    uint8_t cr = rtl_read_byte(CR);

    // Turn the transmitter on
    rtl_write_byte(CR, cr | CR_TE);

    // Set up the TX settings
    rtl_write_long(TCR, dev->tx_config);

    // We want 8 + (3 x 32) bytes = 104 bytes of early TX threshold
    // It means we put data on the wire only once FIFO has reached this threshold
    dev->tx_flags = (3 << TSD_ERTXTH_SHIFT);

    // Inform the hardware about the DMA location of the TX descriptors
    // That way, later it can read the frames we want to send
    for (i = 0; i < TX_MAX_DESC; ++i)
        rtl_write_long(TSAD0 + i * TSAD_GAP, dev->tx->dma_address + i * R8139DN_TX_DESC_SIZE);
}

void rtl8139_setup_rx(rtl8139_dev_t *dev)
{
    uint8_t cr = rtl_read_byte(CR);

    // Disable Multiple Interrupt (we're going to disable early RX mode in RCR)
    rtl_write_word(MULINT, 0);

    // Tell the hardware where to DMA (location of the RX buffer)
    // We do this before enabling RX to avoid the NIC starting DMA before it knows the address
//    rtl_write_long(RBSTART, dev->rx->dma_address);
    rtl_write_long(RBSTART, ((uint32_t)&rx_data - DMA_ADDRESS_OFFSET));


    // Turn the receiver on
    rtl_write_byte(CR, cr | CR_RE);

    // Set up the RX settings
    // We want to receive broadcast frames as well as frames for our own MAC
    rtl_write_long(RCR,  RCR_MXDMA_1024 | RCR_AAP | RCR_AM | RCR_APM | RCR_AB | RCR_RBLEN_16K | RCR_WRAP);
}

void rtl8139_disable_transceiver(rtl8139_dev_t *dev)
{
    rtl_write_byte(CR, 0);
}

// Ask the device to enable interrupts
void rtl8139_enable_irq(rtl8139_dev_t *dev)
{
    rtl_write_word(IMR, dev->interrupt);
}

// Ask the device to disable interrupts
void rtl8139_disable_irq(rtl8139_dev_t *dev)
{
    rtl_write_word(IMR, 0);
}

buffer_t *rtl8139_init_buffer(uint32_t size)
{
    buffer_t *buffer = heap_kernel_alloc(size + sizeof(buffer_t), 4);
    buffer->cur = 0;
    buffer->size = size;
    buffer->dma_address = buffer->data - DMA_ADDRESS_OFFSET;
    return buffer;
}

bool rtl8139_init(net_device_t *net_dev)
{
    // Prevent for re-init
    if (pci_rtl8139_device.vendor_id != 0)
        return false;

    // First get the network device using PCI
    // PCI must be initialized!

    for (int i = 0; i < pci_get_number_of_devices(); i++)
    {
        pci_device *dev = pci_get_device(i);
        if (dev->vendor_id == RTL8139_DEVICE_VENDOR_ID)
        {
            if (dev->device_id == RTL8139_DEVICE_ID ||
                dev->device_id == RTL8139_DEVICE_ID_2 ||
                dev->device_id == RTL8139_DEVICE_ID_SECONDARY)
            {
                pci_rtl8139_device = *dev;
                goto device_found;
            }
        }
    }

    return false;

device_found:
    // Now setup registers, and memory
    rtl8139_device.bar_type = pci_rtl8139_device.base_addres_0 & (0x1);
    rtl8139_device.io_base = pci_rtl8139_device.base_addres_0 & (~0x3);
    rtl8139_device.mem_base = pci_rtl8139_device.base_addres_0 & (~0xf);

    // PCI bus mastering
    pci_busmaster_set(&pci_rtl8139_device, true);

    rtl8139_read_mac();
    net_dev->device_name = device_name;
    memcpy(&net_dev->interface->mac, &rtl8139_device.mac, sizeof(mac_addr_t));
    net_dev->dpi.send = &rtl8139_send;
    rtl8139_net_device = net_dev;
    rtl8139_device.rx = rtl8139_init_buffer(1024 * 1024);
    rtl8139_device.tx = rtl8139_init_buffer(1024 * 1024);

    // Enable IRQ
    rtl8139_device.interrupt_vector = pci_rtl8139_device.interrupt_line;
    idt_attach_interrupt_handler(rtl8139_device.interrupt_vector, rtl8139_irq_handler);
    pic_enable_irq(rtl8139_device.interrupt_vector);

    // Power on device
    rtl_write_byte(CONFIG1, 0x0);

    rtl8139_reset(&rtl8139_device);

    rtl8139_device.interrupt = INT_TX | INT_RX;
    rtl8139_device.tx_config = TCR_IFG_DEFAULT | TCR_MXDMA_1024 | TCR_LBK_DISABLE;

    rtl8139_setup_tx(&rtl8139_device);
    rtl8139_setup_rx(&rtl8139_device);

    rtl8139_enable_irq(&rtl8139_device);

    return true;
}

bool rtl8139_irq_handler()
{
    bool rv = 0;
    rtl8139_disable_irq(&rtl8139_device);

    // Get status of device
    uint16_t isr = rtl_read_word(ISR);

    rtl_write_word(ISR, isr);

    if (isr & INT_RX)
        rtl8139_receive();

out:
    rtl8139_enable_irq(&rtl8139_device);
    return rv;
}

void rtl8139_read_mac()
{
    for (uint8_t i = 0; i < 6; i++)
        rtl8139_device.mac.addr[i] = io_in_byte(rtl8139_device.io_base + i);
}

void rtl8139_get_mac(uint8_t *buffer)
{
    if (!buffer)
        return;

    memcpy(buffer, rtl8139_device.mac.addr, sizeof(mac_addr_t));
}

uint32_t rtl8139_receive()
{
    uint32_t *data_ptr = (uint32_t *)(rx_data + rtl8139_device.rx->cur);

    // Get incoming data length by skipping packet header - 16 LSB bits of header
    uint16_t size = *data_ptr >> 16;

    char tmp[32];
    kernel_sprintf(tmp, "S: %d", size);
    logger_log_info(tmp);

    // Copy received data to buffer, data is right after the header with length
    nic_data_t *out = rtl8139_net_device->dpi.get_receive_buffer(rtl8139_net_device);
    /*
        7 - dword alignment
        RX_READ_POINTER_MASK - avoid overflow
    */
    rtl8139_device.rx->cur = (rtl8139_device.rx->cur + size + 7) & (~3);
    rtl8139_device.rx->cur %= RTL8139_RX_BUFFER_BASE;

    rtl_write_word(CAPR, rtl8139_device.rx->cur);

    // Notify network manager about incoming data
    (*rtl8139_net_device->dpi.receive)(out);
}

uint32_t rtl8139_send(nic_data_t *data)
{

    // uint32_t phys_addr = GET_PHYSICAL_ADDRESS(data->frame);

    // io_out_long(rtl8139_device.io_base + TSAD[rtl8139_device.tx_cur], phys_addr);

    // uint32_t status = 0;
    // status |= data->length & 0x1FFF; // 0-12: Length
    // status |= 0 << 13;               // 13: OWN bit

    // io_out_long(rtl8139_device.io_base + TSD[rtl8139_device.tx_cur], status);

    // while (1)
    // {
    //     // Wait for TOK flag
    //     if (io_in_long(rtl8139_device.io_base + TSD[rtl8139_device.tx_cur]) & TX_TOK)
    //         break;
    // }

    // // Switch buffer
    // rtl8139_device.tx_cur = (rtl8139_device.tx_cur + 1) % 4;
}

void rtl_write_byte(uint32_t address, uint8_t value)
{
    io_out_byte(rtl8139_device.io_base + address, value);
    return 0;
}

uint8_t rtl_read_byte(uint32_t address)
{
    return io_in_byte(rtl8139_device.io_base + address);
}

void rtl_write_word(uint32_t address, uint16_t value)
{
    io_out_word(rtl8139_device.io_base + address, value);
    return 0;
}

uint16_t rtl_read_word(uint32_t address)
{
    return io_in_word(rtl8139_device.io_base + address);
}

void rtl_write_long(uint32_t address, uint32_t value)
{
    io_out_long(rtl8139_device.io_base + address, value);
    return 0;
}

uint32_t rtl_read_long(uint32_t address)
{
    return io_in_byte(rtl8139_device.io_base + address);
}