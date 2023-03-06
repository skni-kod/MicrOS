/*
    @JakubPrzystasz
    Created: 06.02.2021
    Last modified: 25.11.2021
*/
#ifndef RTL8169_H
#define RTL8169_H

#include <stdlib.h>
#include <drivers/pci/pci.h>
#include <assembly/io.h>
#include <cpu/idt/idt.h>
#include <micros/network_device.h>
#include <cpu/dma/dma.h>
#include <klibrary/kvector.h>
#include <memory/heap/heap.h>
#include <inet/inet.h>

#define RTL8169_VENDOR_ID 0x10EC
#define RTL8169_DEVICE_ID 0x8169

#define RTL8169_RX_BUFFER_BASE 8192
#define RTL8169_RX_BUFFER_SIZE (RTL8169_RX_BUFFER_BASE + 16 + 1500)


typedef struct rtl8169_rx_descriptor
{
    union
    {
        uint32_t command;
        struct
        {
            uint32_t
                OWN : 1,
                EOR : 1, : 16,
                length : 14;
        };
    };
    union
    {
        uint32_t vlan;
        struct
        {
            uint32_t : 16,
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
                OWN : 1,
                EOR : 1,
                FS : 1,
                LS : 1,
                LGSEN : 1,
                MSS : 11,
                length : 16;
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
    io_type_t io_type;
    union
    {
        uint16_t io_base;
        uint32_t mem_base;
    };
    //rtl8169_isr_t isr;
    uint32_t irq_vector;
    rtl8169_rx_descriptor_t *rx;
    rtl8169_tx_descriptor_t *tx;
    uint32_t rx_ptr;
    uint32_t tx_ptr;
    net_device_t *net;
    pci_device *pci;
} rtl8169_dev_t;

//! rtl8169_probe
/*!
    Initialize network card driver
*/
bool rtl8169_probe(net_device_t *(*get_net_device)());

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
bool rtl8169_irq_handler(interrupt_state *state);

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