#ifndef RTL8139_REGS_H
#define RTL8139_REGS_H

#include <stdlib.h>

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

#endif