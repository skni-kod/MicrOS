/*
    @JakubPrzystasz
    Created: 06.02.2021
    Last modified: 17.03.2022
*/
#include "rtl8139.h"

pci_device pci_rtl8139_device;
rtl8139_dev_t rtl8139_device;
net_device_t *rtl8139_net_device;

uint32_t current_packet_ptr = 0;

//! Transmit start address of descriptor (device has 4 descriptors)
uint8_t const TSAD[4] = {0x20, 0x24, 0x28, 0x2C};

//! Transmit status of descriptor
uint8_t const TSD[4] = {0x10, 0x14, 0x18, 0x1C};

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
            if (dev->device_id == RTL8139_DEVICE_ID_PRIMARY || dev->device_id == RTL8139_DEVICE_ID_SECONDARY)
            {
                pci_rtl8139_device = *dev;
                break;
            }
        }
    }

    // Device not present in PCI bus
    if (pci_rtl8139_device.vendor_id == 0)
        return false;

    // Now setup registers, and memory
    rtl8139_device.bar_type = pci_rtl8139_device.base_addres_0 & (0x1);
    rtl8139_device.io_base = pci_rtl8139_device.base_addres_0 & (~0x3);
    rtl8139_device.mem_base = pci_rtl8139_device.base_addres_0 & (~0xf);

    // PCI bus mastering
    pci_busmaster_set(&pci_rtl8139_device, true);

    // Power on device
    io_out_byte(rtl8139_device.io_base + CONFIG1, 0x0);

    // Software reset
    io_out_byte(rtl8139_device.io_base + CHIPCMD, 0x10);
    while ((io_in_byte(rtl8139_device.io_base + CHIPCMD) & 0x10) != 0)
        ;

    // Set Active buffer to first
    rtl8139_device.tx_cur = 0;

    // Set ReceiverEnable and TransmitterEnable to HIGH
    io_out_byte(rtl8139_device.io_base + CHIPCMD, 0x0C);

    // Set Transmit and Receive Configuration Registers
    io_out_long(rtl8139_device.io_base + TXCONFIG, 0x03000700);
    io_out_long(rtl8139_device.io_base + RXCONFIG, 0x0300070A);

    // RECEIVE BUFFER
    // Allocate receive buffer
    rtl8139_device.rx_buffer = 0;
    rtl8139_device.rx_buffer = heap_kernel_alloc(RTL8139_RX_BUFFER_SIZE, 0);
    if (!rtl8139_device.rx_buffer)
        return false;

    memset(rtl8139_device.rx_buffer, 0, RTL8139_RX_BUFFER_SIZE);
    io_out_long(rtl8139_device.io_base + RXBUF, (uint32_t)rtl8139_device.rx_buffer - DMA_ADDRESS_OFFSET);

    // (1 << 7) is the WRAP bit, 0xf is (promiscious) AB+AM+APM+AAP
    // WRAP
    // AB - broadcast packets
    // AM - multicast
    // APM - physical match
    // AAP - all packets
    io_out_long(rtl8139_device.io_base + RXCONFIG, 0xF | (1 << 7));

    rtl8139_read_mac();

    // Setup net device
    net_dev->device_name = heap_kernel_alloc((uint32_t)strlen(RTL8139_DEVICE_NAME) + 1, 0);
    strcpy(net_dev->device_name, RTL8139_DEVICE_NAME);
    memcpy(&net_dev->interface->mac, rtl8139_device.mac_addr, sizeof(mac_addr_t));
    net_dev->dpi.send = &rtl8139_send;
    rtl8139_net_device = net_dev;

    // Driver is ready
    //  Set TransmitterOK and ReceiveOK to HIGH
    io_out_word(rtl8139_device.io_base + INTRSTATUS, 0x0);
    io_out_word(rtl8139_device.io_base + INTRMASK, 0xFFFF | 0xE07F);

    // Enable IRQ
    uint32_t irq_num = pci_rtl8139_device.interrupt_line;
    idt_attach_interrupt_handler(irq_num, rtl8139_irq_handler);
    pic_enable_irq(irq_num);

    return true;
}

bool rtl8139_irq_handler()
{
    // Get status of device
    uint16_t status = io_in_word(rtl8139_device.io_base + INTRSTATUS);

    if (status & RXOVW)
    {
        current_packet_ptr = io_in_word(rtl8139_device.io_base + RXBUFADDR) % 8192;
        io_out_word(rtl8139_device.io_base + CAPR, current_packet_ptr - 16);
        io_out_word(rtl8139_device.io_base + INTRSTATUS, 0x1);
        return false;
    }

    if (status & ROK)
        rtl8139_receive();

    io_out_word(rtl8139_device.io_base + INTRSTATUS, 0x5);

    return false;
}

void rtl8139_read_mac()
{
    for (uint8_t i = 0; i < 6; i++)
        rtl8139_device.mac_addr[i] = io_in_byte(rtl8139_device.io_base + i);
}

void rtl8139_get_mac(uint8_t *buffer)
{
    if (!buffer)
        return;

    memcpy(buffer, rtl8139_device.mac_addr, 6);
}

uint32_t rtl8139_receive()
{
    uint32_t *data_ptr = (uint32_t *)(rtl8139_device.rx_buffer + current_packet_ptr);

    // Get incoming data length by skipping packet header - 16 LSB bits of header
    uint16_t size = *data_ptr >> 16;

    // Copy received data to buffer, data is right after the header with length
    nic_data_t *out = rtl8139_net_device->dpi.get_receive_buffer(rtl8139_net_device, size);
    memcpy((void *)out->frame, &data_ptr[1], size);

    /*
        4 - header length (PktLength include 4 bytes CRC)
        3 - dword alignment
        RX_READ_POINTER_MASK - avoid overflow
    */
    current_packet_ptr = (current_packet_ptr + size + 3 + 4) & RX_READ_POINTER_MASK;
    current_packet_ptr %= RTL8139_RX_BUFFER_BASE;
    // Tell to device where put, next incoming packet - 0x10 avoid overflow
    io_out_word(rtl8139_device.io_base + CAPR, current_packet_ptr - 0x10);

    // Notify network manager about incoming data

    (*rtl8139_net_device->dpi.receive)(out);

}

uint32_t rtl8139_send(nic_data_t *data)
{
    uint32_t phys_addr = GET_PHYSICAL_ADDRESS(data->frame);

    io_out_long(rtl8139_device.io_base + TSAD[rtl8139_device.tx_cur], phys_addr);

    uint32_t status = 0;
    status |= data->length & 0x1FFF; // 0-12: Length
    status |= 0 << 13;               // 13: OWN bit

    io_out_long(rtl8139_device.io_base + TSD[rtl8139_device.tx_cur], status);

    while (1)
    {
        // Wait for TOK flag
        if (io_in_long(rtl8139_device.io_base + TSD[rtl8139_device.tx_cur]) & TX_TOK)
            break;
    }

    // Switch buffer
    rtl8139_device.tx_cur = (rtl8139_device.tx_cur + 1) % 4;
}
