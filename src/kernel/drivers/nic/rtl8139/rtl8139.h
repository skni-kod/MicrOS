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
#include <inet/inet.h>

#define RTL8139_VENDOR_ID 0x10EC
#define RTL8139_DEVICE_ID 0x8139

#define RTL8139_RX_BUFFER_BASE 8192
#define RTL8139_RX_BUFFER_SIZE (RTL8139_RX_BUFFER_BASE + 16 + 1500)

typedef struct rtl8139_buffer
{
    uint32_t size;
    uint32_t pointer;
    uint32_t dma_address;
    uint32_t data[];
} rtl8139_buffer_t;

typedef struct rtl8139_dev
{
    io_type_t io_type;
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
bool rtl8139_irq_handler(interrupt_state *state);

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