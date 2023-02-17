/*
    @JakubPrzystasz
    Created: 06.02.2021
    Last modified: 25.11.2021
*/
#ifndef RTL8139_H
#define RTL8139_H

#include <stdlib.h>
#include <drivers/pci/pci.h>
#include <assembly/io.h>
#include <cpu/idt/idt.h>
#include <micros/network_device.h>
#include <cpu/dma/dma.h>
#include <klibrary/kvector.h>

#define RTL8139_DEVICE_VENDOR_ID 0x10EC
#define RTL8139_DEVICE_ID 0x8139
#define RTL8139_DEVICE_ID_2 0x8136
#define RTL8139_DEVICE_ID_SECONDARY 0x8119
#define RTL8139_DEVICE_NAME "RTL8139 NIC"

#define R8139DN_RX_PAD 16
#define R8139DN_RX_HEADER_SIZE 4
#define R8139DN_RX_ALIGN_ADD 3
#define R8139DN_RX_ALIGN_MASK (~R8139DN_RX_ALIGN_ADD)
#define R8139DN_RX_ALIGN(val) (((val) + R8139DN_RX_ALIGN_ADD) & R8139DN_RX_ALIGN_MASK)

#define ETH_FRAME_LEN 1514
#define ETH_ALEN 6
#define ETH_ZLEN 60

/* PCI Tuning Parameters
   Threshold is bytes transferred to chip before transmission starts. */
#define TX_FIFO_THRESH 256        /* In bytes, rounded down to 32 byte units. */
#define RX_FIFO_THRESH 4          /* Rx buffer level before first PCI xfer.  */
#define RX_DMA_BURST 4            /* Maximum PCI burst, '4' is 256 bytes */
#define TX_DMA_BURST 4            /* Calculate as 16<<val. */
#define NUM_TX_DESC 4             /* Number of Tx descriptor registers. */
#define TX_BUF_SIZE ETH_FRAME_LEN /* FCS is added by the chip */
#define RX_BUF_LEN_IDX 0          /* 0, 1, 2 is allowed - 8,16,32K rx buffer */
#define RX_BUF_LEN (8192 << RX_BUF_LEN_IDX)

#define RTL8139_RX_BUFFER_BASE 8192
#define RTL8139_RX_BUFFER_SIZE (RTL8139_RX_BUFFER_BASE + 16 + 1500)

typedef struct buffer{
    uint32_t size;
    uint32_t cur;
    uint32_t dma_address;
    uint8_t data[];
} buffer_t;

typedef struct rtl8139_dev
{
    uint8_t bar_type;
    union
    {
        uint16_t io_base;
        uint32_t mem_base;
    };
    mac_addr_t mac;
    buffer_t *tx;
    buffer_t *rx;
    uint32_t tx_config;
    uint32_t tx_flags;
    uint32_t interrupt;
    uint32_t interrupt_vector;
} rtl8139_dev_t;

struct rx_header
{
    uint16_t status;
    uint16_t size;
};

void rtl_write_byte(uint32_t address, uint8_t value);

uint8_t rtl_read_byte(uint32_t address);

void rtl_write_word(uint32_t address, uint16_t value);

uint16_t rtl_read_word(uint32_t address);

void rtl_write_long(uint32_t address, uint32_t value);

uint32_t rtl_read_long(uint32_t address);

//! rtl8139_init
/*!
    Initialize network card driver
    Requires initialized PCI driver
    If device not present, returns 0, if yes pointer to dev mac address
*/
bool rtl8139_init(net_device_t *net_dev);

//! rtl8139_send
/*
    Send data
*/
uint32_t rtl8139_send(nic_data_t *packet);

//! rtl8139_receive
/*
    Receive data
*/
uint32_t rtl8139_receive();

//! rtl8139_irq_handler
/*!
    Manages interruptions of device
*/
bool rtl8139_irq_handler();

//! rtl8139_read_mac
/*!
    Reads device mac address
*/
void rtl8139_read_mac();

//! rtl8139_get_mac
/*!
    Puts to buffer mac address of device
    Requires buffer with size of (6 * uint8_t)
*/
void rtl8139_get_mac(uint8_t *buffer);

#endif