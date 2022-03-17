/*
    @JakubPrzystasz
    Created: 06.02.2021
    Last modified: 17.03.2022
*/
#include "rtl8139.h"

pci_device pci_rtl8139_device = {0};
rtl8139_dev_t rtl8139_device = {0};
void (*rtl8139_receive_packet)(net_packet_t *);

uint32_t current_packet_ptr = 0;

//! Transmit start address of descritor (device has 4 descriptors)
uint8_t TSAD_array[4] = {0x20, 0x24, 0x28, 0x2C};

//! Transmit status of descriptor
uint8_t TSD_array[4] = {0x10, 0x14, 0x18, 0x1C};

device_configuration_t *rtl8139_configuration;

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
    uint32_t pci_bus_config = pci_io_in(&pci_rtl8139_device, 1);
    pci_bus_config |= 0x4;
    pci_io_out(&pci_rtl8139_device, 1, pci_bus_config);

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
    rtl8139_device.rx_buffer = heap_kernel_alloc(RTL8139_RX_BUFFER_SIZE, 0);
    memset(rtl8139_device.rx_buffer, 0, RTL8139_RX_BUFFER_SIZE);
    io_out_long(rtl8139_device.io_base + RXBUF, (uint32_t)rtl8139_device.rx_buffer - DMA_ADDRESS_OFFSET);

    // (1 << 7) is the WRAP bit, 0xf is (promiscious) AB+AM+APM+AAP
    // WRAP
    // AB - broadcast packets
    // AM - multicast
    // APM - physical match
    // AAP - all packets
    io_out_long(rtl8139_device.io_base + RXCONFIG, 0xF | (1 << 7));

    // Set TransmitterOK and ReceiveOK to HIGH
    io_out_word(rtl8139_device.io_base + INTRSTATUS, 0x0);
    io_out_word(rtl8139_device.io_base + INTRMASK, 0xFF);

    // Enable IRQ
    uint32_t irq_num = pci_rtl8139_device.interrupt_line;
    pic_enable_irq(irq_num);
    idt_attach_interrupt_handler(irq_num, rtl8139_irq_handler);

    rtl8139_read_mac();

    net_dev->device_name = heap_kernel_alloc((uint32_t)strlen(RTL8139_DEVICE_NAME) + 1, 0);
    strcpy(net_dev->device_name, RTL8139_DEVICE_NAME);
    memcpy(net_dev->mac_address, rtl8139_device.mac_addr, sizeof(uint8_t) * 6);
    net_dev->send_packet = &rtl8139_send;
    rtl8139_receive_packet = net_dev->receive_packet;
    rtl8139_configuration = net_dev->configuration;

    return true;
}

bool rtl8139_irq_handler()
{
    // Get status of device
    uint16_t status = io_in_word(rtl8139_device.io_base + INTRSTATUS);

    if (status & ROK && rtl8139_configuration->mode & 0x1)
        rtl8139_receive();

    io_out_word(rtl8139_device.io_base + INTRSTATUS, 0x5);

    return true;
}

void rtl8139_read_mac()
{
    for (uint8_t i = 0; i < 6; i++)
        rtl8139_device.mac_addr[i] = io_in_byte(rtl8139_device.io_base + i);
}

void rtl8139_get_mac(uint8_t *buffer)
{
    if (buffer == 0)
        return;

    memcpy(buffer, rtl8139_device.mac_addr, 6);
}

void rtl8139_receive()
{
    uint32_t *packet = (uint16_t *)(rtl8139_device.rx_buffer + current_packet_ptr);

    // Get packet length by skipping packet header
    uint32_t packet_length = *(packet + 1);

    // Copy received data to heap
    void *packet_data = heap_kernel_alloc(packet_length, 0);
    memcpy(packet_data, packet + 2, packet_length);

    // Add packet to packets queue
    net_packet_t *out = heap_kernel_alloc(sizeof(net_packet_t), 0);
    out->packet_data = packet_data;
    out->packet_length = packet_length;
    rtl8139_get_mac(out->device_mac);

    current_packet_ptr = (current_packet_ptr + packet_length + 7) & RX_READ_POINTER_MASK;

    if (current_packet_ptr > RTL8139_RX_BUFFER_SIZE)
        current_packet_ptr -= RTL8139_RX_BUFFER_SIZE;

    // Tell to device where put, next incoming packet
    io_out_word(rtl8139_device.io_base + CAPR, current_packet_ptr - 0x10);

    (*rtl8139_receive_packet)(out);
}

void rtl8139_send(net_packet_t *packet)
{
    if (rtl8139_configuration->mode & 0x2)
    {
        void *data = heap_kernel_alloc(packet->packet_length, 0);
        memcpy(data, packet->packet_data, packet->packet_length);
        void *phys_addr = (void *)((uint32_t)data - DMA_ADDRESS_OFFSET);

        io_out_long(rtl8139_device.io_base + TSAD_array[rtl8139_device.tx_cur], (uint32_t)phys_addr);

        uint32_t status = 0;
        status |= packet->packet_length & 0x1FFF; // 0-12: Length
        status |= 0 << 13;                        // 13: OWN bit
        io_out_long(rtl8139_device.io_base + TSD_array[rtl8139_device.tx_cur], status);

        while (true)
        {
            // Wait for TOK flag
            if (io_in_long(rtl8139_device.io_base + TSD_array[rtl8139_device.tx_cur]) & TX_TOK)
                break;
        }

        // Switch buffer
        rtl8139_device.tx_cur++;
        if (rtl8139_device.tx_cur > 3)
            rtl8139_device.tx_cur = 0;

        // Dealloc transmit data buffer
        heap_kernel_dealloc(data);
    }
}
