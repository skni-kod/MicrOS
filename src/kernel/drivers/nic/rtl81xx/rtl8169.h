/*
    @JakubPrzystasz
    Created: 06.02.2021
    Last modified: 25.11.2021
*/
#ifndef RTL8169_H
#define RTL8169_H

#include <inet/inet.h>
#include <stdlib.h>
#include <drivers/pci/pci.h>
#include <assembly/io.h>
#include <cpu/idt/idt.h>
#include <micros/network_device.h>
#include <cpu/dma/dma.h>
#include <klibrary/kvector.h>
#include <memory/heap/heap.h>

#define rtl8169_VENDOR_ID 0x10EC
#define rtl8169_DEVICE_ID_A 0x8139
#define rtl8169_DEVICE_ID_B 0x8136
#define rtl8169_DEVICE_ID_C 0x0139

#define rtl8169_RX_BUFFER_BASE 8192
#define rtl8169_RX_BUFFER_SIZE (rtl8169_RX_BUFFER_BASE + 16 + 1500)

typedef enum rtl8169_registers
{
    IDR0 = 0x00,
    MAR0 = 0x08,
    DTCCR = 0x10,
    TNPDS_LOW = 0x20,
    TNPDS_HIGH = 0x24,
    THPDS = 0x28,
    FLASH = 0x30,
    ERBCR = 0x34,
    ERSR = 0x36,
    CR = 0x37,
    TPPoll = 0x38,
    IMR = 0x3C,
    ISR = 0x3E,
    TCR = 0x40,
    RCR = 0x44,
    TCTR = 0x48,
    MPC = 0x4C,
    CR9346 = 0x50,
    CONFIG0 = 0x51,
    CONFIG1 = 0x52,
    CONFIG2 = 0x53,
    CONFIG3 = 0x54,
    CONFIG4 = 0x55,
    CONFIG5 = 0x56,
    TimerInt = 0x58,
    MULINT = 0x5C,
    PHYAR = 0x60,
    TBICSR0 = 0x64,
    TBI_ANAR = 0x68,
    TBI_LPAR = 0x6A,
    RDSA_LOW = 0xE4,
    RDSA_HIGH = 0xE8,
    ETThR = 0xEC
} rtl8169_registers_t;

typedef union rtl8169_cr
{
    uint8_t value;
    struct
    {
        uint8_t : 2,
            TE : 1,
            RE : 1,
            RST : 1, : 3;
    };
} rtl8169_cr_t;

typedef union rtl8169_txcfg
{
    uint32_t value;
    struct
    {
        uint32_t : 8,
            MXDMA : 3, : 5,
            CRC : 1,
            LBK : 2,
            IFG2 : 1, : 3,
            HWVERID1 : 1,
            IFG1 : 2,
            HWVERID2 : 5, : 1;
    };
} rtl8169_txcfg_t;

typedef union rtl8169_rxcfg
{
    uint32_t value;
    struct
    {
        uint32_t
            AAP : 1,
            APM : 1,
            AM : 1,
            AB : 1,
            AR : 1,
            AER : 1,
            SEL9356 : 1, : 1,
            MXDMA : 3, : 2,
            RXFTH : 3,
            RER8 : 1, : 7,
            MULERTH : 1, : 7;
    };
} rtl8169_rxcfg_t;

typedef union rtl8169_imr
{
    uint16_t value;
    struct
    {
        uint16_t
            ROK : 1,
            RER : 1,
            TOK : 1,
            TER : 1,
            RDU : 1,
            PUN_LINK : 1,
            FOVW : 1,
            TDU : 1,
            SWInt : 1, : 5,
            TimeOut : 1,
            SERR : 1;
    };
} rtl8169_imr_t;

typedef rtl8169_imr_t rtl8169_isr_t;

typedef union rtl8169_9346cr
{
    uint8_t value;
    struct
    {
        uint8_t
            EEDO : 1,
            EEDI : 1,
            EESK : 1,
            EECS : 1, : 2,
            EEM : 2
    };
} rtl8169_9346cr_t;

typedef union rtl8169_config1
{
    uint8_t value;
    struct
    {
        uint8_t
            PMEn : 1,
            VPD : 1,
            IOMAP : 1,
            MEMMAP : 1,
            LWACT : 1,
            DVRLOAD : 1,
            LEDS : 2;
    };
} rtl8169_config1_t;

typedef enum rtl8169_operating_mode
{
    RTL8169_NORMAL = 0b00,
    RTL8169_AUTO_LOAD = 0b01,
    RTL8169_PROGRAMMING = 0b10,
    RTL8169_CONFIG = 0b11,
} rtl8169_operating_mode_t;

typedef union rtl8169_etthr
{
    uint8_t value;
    struct
    {
        uint8_t
            ETTh : 6,
             : 2;
    };
} rtl8169_etthr_t;

typedef struct rtl8169_rx_descriptor
{
    union
    {
        uint32_t;
        struct
        {
            uint32_t
                length : 14,
                : 16,
                EOR : 1,
                OWN : 1;
        };
    };
    union
    {
        uint32_t;
        struct
        {
            uint32_t
                vlan : 16,
                tava : 1, : 15;
        };
    };
    uint32_t low_address;
    uint32_t high_address;
} rtl8169_rx_descriptor_t;

typedef struct rtl8169_tx_descriptor
{
    union
    {
        uint32_t;
        struct
        {
            uint32_t
                length : 16,
                MSS : 11,
                LGSEN : 1,
                LS : 1,
                FS : 1,
                EOR : 1,
                OWN : 1;
        };
    };
    union
    {
        uint32_t;
        struct
        {
            uint32_t
                vlan : 16,
                : 1,
                tagc : 1, : 14;
        };
    };
    uint32_t low_address;
    uint32_t high_address;
} rtl8169_tx_descriptor_t;

typedef struct rtl8169_dev
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
    pci_device *pci;
    rtl8169_isr_t isr;
    uint32_t irq_vector;
    rtl8169_rx_descriptor_t *rx;
    rtl8169_tx_descriptor_t *tx;
    uint32_t rx_ptr;
    uint32_t tx_ptr;
} rtl8169_dev_t;

//! rtl8169_init
/*!
    Initialize network card driver
    Requires initialized PCI driver
*/
uint32_t rtl8169_probe(net_dpi_t *dpi);

//! rtl8169_send
/*
    Send data
*/
uint32_t rtl8169_send(nic_data_t *data);

//! rtl8169_receive
/*
    Receive data
*/
uint32_t rtl8169_receive(rtl8169_dev_t *device);

//! rtl8169_irq_handler
/*!
    Manages interruptions of device
*/
bool rtl8169_irq_handler(void);

//! rtl_read_mac
/*!
    Reads device mac address
*/
void rtl8169_read_mac(rtl8169_dev_t *device);

const char *rtl8169_get_name(rtl8169_dev_t *device);

void rtl8169_write_byte(rtl8169_dev_t *device, uint32_t address, uint8_t value);

uint8_t rtl8169_read_byte(rtl8169_dev_t *device, uint32_t address);

void rtl8169_write_word(rtl8169_dev_t *device, uint32_t address, uint16_t value);

uint16_t rtl8169_read_word(rtl8169_dev_t *device, uint32_t address);

void rtl8169_write_long(rtl8169_dev_t *device, uint32_t address, uint32_t value);

uint32_t rtl8169_read_long(rtl8169_dev_t *device, uint32_t address);

#endif