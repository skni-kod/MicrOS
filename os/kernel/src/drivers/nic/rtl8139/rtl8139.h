/*
    @JakubPrzystasz
    Created: 06.02.2021
    Modify: 16.02.2021
*/
#ifndef RTL8139_H
#define RTL8139_H

#include "../../pci/pci.h"
#include "../../../assembly/io.h"
#include "../../../logger/logger.h"
#include "../../../cpu/idt/idt.h"
#include "../../serial/serial.h"
#include "../../../network/network_definitions.h"
#include "../../../klibrary/kvector.h"
#include <stdlib.h>

#define DEVICE_VENDOR_ID 0x10EC
#define DEVICE_ID 0x8139
#define DEVICE_NAME "RTL8139 NIC"

#define RX_BUFFER_SIZE 8192 + 16 + 1500

#define CAPR 0x38
#define RX_READ_POINTER_MASK (~3)
#define ROK (1 << 0)
#define RER (1 << 1)
#define TOK (1 << 2)
#define TER (1 << 3)
#define TX_TOK (1 << 15)

enum RTL8139_registers
{
    MAG0 = 0x00,      // Ethernet hardware address MAC
    MAR0 = 0x08,      // Multicast
    TXSTATUS0 = 0x10, // TX status (4x32bit)
    TXADDR0 = 0x20,   // TX descriptors (4x32bit)
    RXBUF = 0x30,
    RXEARLYCOUNT = 0x34,
    RXEARLYSTATUS = 0x36,
    CHIPCMD = 0x37,
    RXBUFPTR = 0x38,
    RXBUFADDR = 0x3A,
    INTRMASK = 0x3C,
    INTRSTATUS = 0x3E,
    TXCONFIG = 0x40,
    RXCONFIG = 0x44,
    TIMER = 0x48,    // A general-purpose counter
    RXMISSED = 0x4C, // 24 bits valid, write clears
    CFG9346 = 0x50,
    CONFIG0 = 0x51,
    CONFIG1 = 0x52,
    FLASHREG = 0x54,
    GPPINDATA = 0x58,
    GPPINDIR = 0x59,
    MII_SMI = 0x5A,
    HLTCLK = 0x5B,
    MULTIINTR = 0x5C,
    TXSUMMARY = 0x60,
    MII_BMCR = 0x62,
    MII_BMSR = 0x64,
    NWAYADVERT = 0x66,
    NWAYLPAR = 0x68,
    NWAYEXPANSION = 0x6A,

    // Undocumented registers, but required for proper operation
    FIFOTMS = 0x70, // FIFO Control and test
    CSCR = 0x74,    // Chip Status and Configuration Register
    PARA78 = 0x78,
    PARA7C = 0x7C, // Magic transceiver parameter register
};

typedef struct rtl8139_dev
{
    uint8_t bar_type;
    uint16_t io_base;
    uint32_t mem_base;
    int eeprom_exist;
    uint8_t mac_addr[6];
    char *rx_buffer;
    int tx_cur;
} rtl8139_dev_t;

//! rtl8139_init
/*!
    Initialize network card driver 
    Requires initialized PCI driver
    If device not present, returns 0, if yes pointer to dev mac address
*/
bool rtl8139_init(net_device_t *net_dev);

//! rtl8139_send_packet
/*
    Send data, with desired length
*/
void rtl8139_send_packet(net_packet_t *packet);

//! rtl8139_receive_packet
/*
    Receive packet
*/
void rtl8139_receive_packet();

//! rtl8139_irq_handler
/*!
    Manages interruptions of device
*/
bool rtl8139_irq_handler();

//! rtl8139_read_mac_addr
/*!
    Reads device mac address
*/
void rtl8139_read_mac_addr();

//! rtl8139_get_mac_addr
/*!
    Puts to buffer mac address of device 
    Requires buffer with size of (6 * uint8_t)
*/
void rtl8139_get_mac_addr(uint8_t *buffer);

//! rtl8139_get_sent_count
/*!
    Returns number of sent packets
*/

uint32_t rtl8139_get_sent_count();

//! rtl8139_get_received_count
/*!
    Returns number of received packets
*/
uint32_t rtl8139_get_received_count();

#endif