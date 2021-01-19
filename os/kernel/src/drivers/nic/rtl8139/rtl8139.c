#include "rtl8139.h"

pci_device pci_rtl8139_device = {0};
rtl8139_dev_t rtl8139_device = {0};

uint32_t current_packet_ptr;

//! Transmit start address of descritor (device has 4 descriptors)
uint8_t TSAD_array[4] = {0x20, 0x24, 0x28, 0x2C};

//! Transmit status of descriptor
uint8_t TSD_array[4] = {0x10, 0x14, 0x18, 0x1C};

void rtl8139_init()
{
    //Prevent for re-init
    if (pci_rtl8139_device.vendor_id != 0)
    {
        return;
    }

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
    {
        logger_log_info("NIC not found\n");
        return;
    }

    //Now setup registers, and memory
    rtl8139_device.bar_type = pci_rtl8139_device.base_addres_0 & (0x1);
    rtl8139_device.io_base = pci_rtl8139_device.base_addres_0 & (~0x3);
    rtl8139_device.mem_base = pci_rtl8139_device.base_addres_0 & (~0xf);

    rtl8139_device.tx_cur = 0;

    uint32_t pci_command_reg = pci_rtl8139_device.command;
    if (!(pci_command_reg & TOK))
    {
        pci_command_reg |= TOK;
        io_out_byte(pci_rtl8139_device.command, pci_command_reg);
    }

    //Power on device
    io_out_byte(rtl8139_device.io_base + CONFIG1, 0x0);

    //Software reset
    io_out_byte(rtl8139_device.io_base + CHIPCMD, 0x10);
    while ((io_in_byte(rtl8139_device.io_base + CHIPCMD) & 0x10) != 0)
    {
        ;
    }

    //TODO: MEMORY ALLOCATION IS OK ?
    // Allocate receive buffer
    rtl8139_device.rx_buffer = heap_kernel_alloc(RX_BUFFER_SIZE, 0);
    memset(rtl8139_device.rx_buffer, 0x0, RX_BUFFER_SIZE);
    io_out_long(rtl8139_device.io_base + RXBUF, rtl8139_device.rx_buffer + 0xC0000000);

    // Set TransmitterOK and ReceiverOK to HIGH
    io_out_word(rtl8139_device.io_base + INTRMASK, 0x0005);

    // (1 << 7) is the WRAP bit, 0xf is AB+AM+APM+AAP
    io_out_long(rtl8139_device.io_base + RXCONFIG, 0xf | (1 << 7));

    // Set ReceiverEnable and TransmitterEnable to HIGH
    io_out_byte(rtl8139_device.io_base + CHIPCMD, 0x0C);

    // Enable interruptions
    uint32_t irq_num = pci_rtl8139_device.interrupt_line;
    idt_attach_interrupt_handler(32 + irq_num, rtl8139_irq_handler);

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

    //Finally we can tell that - We did it!
    logger_log_ok(logInfo);
}

void rtl8139_irq_handler(pci_in_data *data)
{
    //Get status of device
    uint16_t status = io_in_word(rtl8139_device.io_base + INTRSTATUS);

    if (status & TOK)
    {
        //logger_log_info("Packet sent");
    }
    if (status & ROK)
    {
        //logger_log_info("Packet received");
        //receive_packet();
    }

    io_out_word(rtl8139_device.io_base + INTRSTATUS, 0x5);
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