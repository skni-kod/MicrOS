#include "rtl8139.h"

pci_device pci_rtl8139_device = {0};
rtl8139_dev_t rtl8139_device = {0};

uint32_t current_packet_ptr;

uint32_t sent_count = 0;
uint32_t received_count = 0;

//! Transmit start address of descritor (device has 4 descriptors)
uint8_t TSAD_array[4] = {0x20, 0x24, 0x28, 0x2C};

//! Transmit status of descriptor
uint8_t TSD_array[4] = {0x10, 0x14, 0x18, 0x1C};

void rtl8139_init()
{
    //Prevent for re-init
    if (pci_rtl8139_device.vendor_id != 0)
        return;

    // First get the network device using PCI
    // PCI must be initialized!

    for (int i = 0; i < pci_get_number_of_devices(); i++)
    {
        pci_device *dev = pci_get_device(i);
        if (dev->vendor_id == DEVICE_VENDOR_ID && dev->device_id == DEVICE_ID)
        {
            pci_rtl8139_device = *dev;
            break;
        }
    }

    if (pci_rtl8139_device.vendor_id == 0)
        return;

    //Now setup registers, and memory
    rtl8139_device.bar_type = pci_rtl8139_device.base_addres_0 & (0x1);
    rtl8139_device.io_base = pci_rtl8139_device.base_addres_0 & (~0x3);
    rtl8139_device.mem_base = pci_rtl8139_device.base_addres_0 & (~0xf);

    //PCI bus mastering
    uint32_t pci_bus_config = pci_io_in(&pci_rtl8139_device, 1);
    pci_bus_config |= 0x4;
    pci_io_out(&pci_rtl8139_device, 1, pci_bus_config);

    //Power on device
    io_out_byte(rtl8139_device.io_base + CONFIG1, 0x0);

    //Software reset
    io_out_byte(rtl8139_device.io_base + CHIPCMD, 0x10);
    while ((io_in_byte(rtl8139_device.io_base + CHIPCMD) & 0x10) != 0)
        ;

    //Set Active buffer to first
    rtl8139_device.tx_cur = 0;

    // Set ReceiverEnable and TransmitterEnable to HIGH
    io_out_byte(rtl8139_device.io_base + CHIPCMD, 0x0C);

    //Set Transmit and Receive Configuration Registers
    io_out_long(rtl8139_device.io_base + TXCONFIG, 0x03000700);
    io_out_long(rtl8139_device.io_base + RXCONFIG, 0x0300070A);

    //RECEIVE BUFFER
    // Allocate receive buffer
    rtl8139_device.rx_buffer = heap_kernel_alloc(RX_BUFFER_SIZE, 0);
    memset(rtl8139_device.rx_buffer, 0x0, RX_BUFFER_SIZE);
    io_out_long(rtl8139_device.io_base + RXBUF, (uint32_t)rtl8139_device.rx_buffer - 0xC0000000);

    // (1 << 7) is the WRAP bit, 0xf is AB+AM+APM+AAP
    io_out_long(rtl8139_device.io_base + RXCONFIG, 0xf | (1 << 7));

    rtl8139_read_mac_addr();

    char logInfo[27] = "NIC MAC: __:__:__:__:__:__";
    char numberBuffer[3];
    for (uint8_t i = 0; i < 6; i++)
    {
        itoa(rtl8139_device.mac_addr[i], numberBuffer, 16);
        //Add leading zero
        if (numberBuffer[1] == 0)
        {
            numberBuffer[1] = numberBuffer[0];
            numberBuffer[0] = '0';
        }
        //Copy just 2 characters, we dont need ending 0
        memcpy(logInfo + (9 + i * 3), numberBuffer, 2);
    }

    //Set TransmitterOK and ReceiveOK to HIGH
    io_out_word(rtl8139_device.io_base + INTRSTATUS, 0x0);
    io_out_word(rtl8139_device.io_base + INTRMASK, 0xff);
    uint32_t irq_num = pci_rtl8139_device.interrupt_line;
    pic_enable_irq(irq_num);
    idt_attach_interrupt_handler(irq_num, rtl8139_irq_handler);

    //Finally we can tell that - We did it!
    logger_log_ok(logInfo);
}

bool rtl8139_irq_handler()
{
    //Get status of device
    uint16_t status = io_in_word(rtl8139_device.io_base + INTRSTATUS);

    logger_log_info("Hey! I've got something for you");

    if (status & TOK)
        sent_count++;

    if (status & ROK)
    {
        received_count++;
        rtl8139_receive_packet();
    }

    io_out_word(rtl8139_device.io_base + INTRSTATUS, 0x5);

    return true;
}

void rtl8139_read_mac_addr()
{
    for (uint8_t i = 0; i < 6; i++)
    {
        rtl8139_device.mac_addr[i] = io_in_byte(rtl8139_device.io_base + i);
    }
}

void rtl8139_get_mac_addr(uint8_t *buffer)
{
    if (buffer == 0)
    {
        return;
    }

    memcpy(buffer, rtl8139_device.mac_addr, 6);
}

void rtl8139_receive_packet()
{
    uint16_t *t = (uint16_t *)(rtl8139_device.rx_buffer + current_packet_ptr);
    // Skip packet header, get packet length
    uint16_t packet_length = *(t + 1);

    // Skip, packet header and packet length, now t points to the packet data
    t = t + 2;

    //Copy received to heap
    void *packet = heap_kernel_alloc(packet_length, 0);
    memcpy(packet, t, packet_length);
    //TODO: PACKET HANDLING

    current_packet_ptr = (current_packet_ptr + packet_length + 4 + 3) & RX_READ_POINTER_MASK;

    if (current_packet_ptr > RX_BUFFER_SIZE)
        current_packet_ptr -= RX_BUFFER_SIZE;

    io_out_word(rtl8139_device.io_base + CAPR, current_packet_ptr - 0x10);
}

void rtl8139_send_packet(void *data, uint32_t len)
{
    void *transfer_data = heap_kernel_alloc(len, 0);
    memcpy(transfer_data, data, len);
    void *phys_addr = (uint32_t)transfer_data - (0xC0000000);

    io_out_long(rtl8139_device.io_base + TSAD_array[rtl8139_device.tx_cur], phys_addr);

    uint32_t status = 0;
    status |= len & 0x1FFF; // 0-12: Length
    status |= 0 << 13;      // 13: OWN bit
    io_out_long(rtl8139_device.io_base + TSD_array[rtl8139_device.tx_cur], status);

    while (true)
    {
        //TODO:Transmit FIFO Underrun
        //Wait for TOK flag
        //TODO: TRANSMISSION TIMEOUT, packet queue
        if (io_in_long(rtl8139_device.io_base + TSD_array[rtl8139_device.tx_cur]) & TX_TOK)
            break;
    }

    //Switch buffer
    rtl8139_device.tx_cur++;
    if (rtl8139_device.tx_cur > 3)
    {
        rtl8139_device.tx_cur = 0;
    }

    //TODO: DEALLOC BUFFER
    heap_kernel_dealloc(transfer_data);
}

uint32_t rtl8139_get_sent_count()
{
    return sent_count;
}

uint32_t rtl8139_get_received_count()
{
    return received_count;
}