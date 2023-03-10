#ifndef RTL8169_REGS_H
#define RTL8169_REGS_H

#include <stdlib.h>

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
    ETThR = 0xEC,
    RMS = 0xDA,
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

typedef union rtl8169_tppoll
{
    uint8_t value;
    struct
    {
        uint8_t
            FSWInt : 1,
            : 5,
            NPQ : 1,
            HPQ : 1;
    };
} rtl8169_tppoll_t;

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

typedef union rtl8169_rms
{
    uint16_t value;
    struct
    {
        uint16_t
            RMS : 14,
            : 2;
    };
} rtl8169_rms_t;

#endif