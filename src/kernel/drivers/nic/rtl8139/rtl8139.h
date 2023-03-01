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
#define RTL8139_DEVICE_ID 0x8139

#define RTL8139_RX_BUFFER_BASE 8192
#define RTL8139_RX_BUFFER_SIZE (RTL8139_RX_BUFFER_BASE + 16 + 1500)

typedef enum rtl8139_registers
{
    IDR0 = 0x00,
    MAR0 = 0x08,
    TSD0 = 0x10,
    TSAD0 = 0x20,
    RBSTART = 0x30,
    ERBCR = 0x34,
    ERSR = 0x36,
    CR = 0x37,
    CAPR = 0x38,
    CBR = 0x3A,
    IMR = 0x3C,
    ISR = 0x3E,
    TCR = 0x40,
    RCR = 0x44,
    TCTR = 0x48,
    MPC = 0x4C,
    CR9346 = 0x50,
    CONFIG0 = 0x51,
    CONFIG1 = 0x52,
    TimerInt = 0x54,
    MSR = 0x58,
    CONFIG3 = 0x59,
    CONFIG4 = 0x5A,
    MULTINT = 0x5C,
    RERID = 0x5E,
    TSAD = 0x60,
    CONFIG5 = 0xD8
} rtl8139_registers_t;

typedef struct rtl8139_rx_header
{
    union
    {
        uint32_t header;
        struct
        {
            uint16_t
                ROK : 1,
                FAE : 1,
                CRC : 1,
                LONG : 1,
                RUNT : 1,
                ISE : 1, : 7,
                BAR : 1,
                PAM : 1,
                MAR : 1;
            uint16_t size;
        };
    };
    uint32_t data[];
} rtl8139_rx_header;

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

typedef union rtl8139_rxcfg
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
            AER : 1, : 1,
            WRAP : 1,
            MXDMA : 3,
            RBLEN : 2,
            RXFTH : 3,
            RER8 : 1,
            MulERINT : 1, : 6,
            ERTH : 4, : 4;
    };
} rtl8139_rxcfg_t;

typedef union rtl8139_cr
{
    uint8_t value;
    struct
    {
        uint8_t
            BUFE : 1,
            : 1,
            TE : 1,
            RE : 1,
            RST : 1, : 3;
    };
} rtl8139_cr_t;

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

typedef union rtl8139_config1
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
} rtl8139_config1_t;

typedef union rtl8139_config4
{
    uint8_t value;
    struct
    {
        uint8_t
            PBWakeup : 1,
            : 1,
            LWPTN : 1, : 1,
            LWPME : 1,
            LongWF : 1,
            AnaOFF : 1,
            RxFIFOAutoClr : 1;
    };
} rtl8139_config4_t;

typedef union rtl8139_media_status_reg
{
    uint8_t value;
    struct
    {
        uint8_t
            RXPF : 1,
            TXPF : 1,
            LNKB : 1,
            SPEED_10 : 1,
            Aux_Status : 1, : 1,
            RXFCE : 1,
            TXFCE : 1;
    };
} rtl8139_media_status_reg_t;

typedef union rtl8139_9346cr
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
} rtl8139_9346cr_t;

typedef enum rtl8139_operating_mode
{
    RTL8139_NORMAL = 0b00,
    RTL8139_AUTO_LOAD = 0b01,
    RTL8139_PROGRAMMING = 0b10,
    RTL8139_CONFIG = 0b11,
} rtl8139_operating_mode_t;

typedef struct rtl8139_hw_ver
{
    union
    {
        struct
        {
            uint8_t
                hw_ver_b : 1,
                hw_ver_a : 5, : 2;
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
    uint32_t tsad;
    rtl8139_buffer_t *rx;
    uint32_t irq_status;
    uint32_t irq_vector;
    net_device_t *net;
    pci_device *pci;
} rtl8139_dev_t;

//! rtl8139_init
/*!
    Initialize network card driver
    Requires initialized PCI driver
    If device not present, returns 0, if yes pointer to dev mac address
*/
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

rtl8139_buffer_t *rtl8139_init_buffer(uint32_t size);

#endif