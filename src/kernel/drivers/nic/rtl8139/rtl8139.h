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
#include <memory/heap/heap.h>

#define RTL8139_VENDOR_ID 0x10EC
#define RTL8139_DEVICE_ID_A 0x8139
#define RTL8139_DEVICE_ID_B 0x8136
#define RTL8139_DEVICE_ID_C 0x0139

#define RTL8139_RX_BUFFER_BASE 8192
#define RTL8139_RX_BUFFER_SIZE (RTL8139_RX_BUFFER_BASE + 16 + 1500)

typedef enum rtl8139_registers
{
    IDR0 = 0x00,      // Ethernet hardware address MAC
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
} rtl8139_registers_t;

typedef union rtl8139_txcfg
{
    uint32_t value;
    struct
    {
        uint32_t
            CLRABT : 1,
            : 3,
            TXRR : 4,
            MXDMA2 : 3, : 5,
            CRC : 1,
            LOOPBACK : 2,
            IFG2 : 1, : 3,
            HWVERID_B : 1,
            IFG : 2,
            HWVERID_A : 5, : 1;
    };
} rtl8139_txcfg_t;

typedef union rtl8139_imr
{
    uint16_t value;
    struct
    {
        uint16_t
            ROK : 1,
            RER : 1,
            TOK : 1,
            TER : 1,
            RXOVW : 1,
            PUN_LINK : 1,
            FOVW : 1, : 6,
            LenChg : 1,
            TimeOut : 1,
            SERR : 1;
    };
} rtl8139_imr_t;

typedef rtl8139_imr_t rtl8139_isr_t;

typedef union rtl8139_tsd
{
    uint32_t value;
    struct
    {
        uint32_t
            SIZE : 13,
            OWN : 1,
            TUN : 1,
            TOK : 1,
            ERTXTH : 6, : 2,
            NCC : 4,
            CDH : 1,
            OWC : 1,
            TABT : 1,
            CRS : 1;
    };
} rtl8139_tsd_t;

typedef struct rtl8139_hw_ver
{
    union
    {
        struct
        {
            uint8_t 
                hw_ver_b : 1,   
                hw_ver_a : 5,
                : 2;
        };
        uint8_t value;
    };
    const char const *string;
} rtl8139_hw_ver_t;

typedef struct rtl8139_buffer
{
    uint32_t size;
    uint32_t pointer;
    uint32_t dma_address;
    uint32_t data[];
} rtl8139_buffer_t;

typedef struct rtl8139_dev
{
    enum
    {
        MMAP,
        IO
    } io_type;
    union
    {
        uint16_t io_base;
        uint32_t mem_base;
    };
    net_device_t *dev;
    uint32_t tsad;
    rtl8139_buffer_t *rx;
    uint32_t irq_status;
    uint32_t irq_vector;
    net_device_t *net_dev;
    pci_device *pci;
} rtl8139_dev_t;

//! rtl8139_init
/*!
    Initialize network card driver
    Requires initialized PCI driver
    If device not present, returns 0, if yes pointer to dev mac address
*/
bool rtl8139_init(net_device_t *(*get_net_device)());

bool rtl8139_probe(net_device_t *(*get_net_device)());

//! rtl8139_send
/*
    Send data
*/
uint32_t rtl8139_send(nic_data_t *data);

//! rtl8139_receive
/*
    Receive data
*/
uint32_t rtl8139_receive(rtl8139_dev_t *device);

//! rtl8139_irq_handler
/*!
    Manages interruptions of device
*/
bool rtl8139_irq_handler(void);

//! rtl_read_mac
/*!
    Reads device mac address
*/
void rtl8139_read_mac(rtl8139_dev_t *device);

const char *rtl8139_get_name(rtl8139_dev_t *device);

void rtl8139_write_byte(rtl8139_dev_t *device, uint32_t address, uint8_t value);

uint8_t rtl8139_read_byte(rtl8139_dev_t *device, uint32_t address);

void rtl8139_write_word(rtl8139_dev_t *device, uint32_t address, uint16_t value);

uint16_t rtl8139_read_word(rtl8139_dev_t *device, uint32_t address);

void rtl8139_write_long(rtl8139_dev_t *device, uint32_t address, uint32_t value);

uint32_t rtl8139_read_long(rtl8139_dev_t *device, uint32_t address);

rtl8139_buffer_t* rtl8139_init_buffer(uint32_t size);

#endif