#ifndef PCI_H
#define PCI_H

#include <stdint.h>

typedef union pci_in_data {
    uint32_t bits;
    struct {
        uint32_t always_zero    : 2;
        uint32_t register_num      : 6;
        uint32_t function_num   : 3;
        uint32_t device_num     : 5;
        uint32_t bus_num        : 8;
        uint32_t reserved       : 7;
        uint32_t enable         : 1;
    };
} pci_in_data;

typedef union pci_dev {
    uint8_t bits_8[64];
    uint16_t bits_16[32];
    uint32_t bits_32[16];
    struct {
        uint16_t vendor_id;
        uint16_t device_id;
        uint16_t command;
        uint16_t status;
        uint8_t revision_id;
        uint8_t prog_if;
        uint8_t subclass;
        uint8_t class_code;
        uint8_t cache_line_size;
        uint8_t latency_timer;
        uint8_t header_type;
        uint8_t bist;
        uint32_t base_addres_0;
        uint32_t base_addres_1;
        uint32_t base_addres_2;
        uint32_t base_addres_3;
        uint32_t base_addres_4;
        uint32_t base_addres_5;
        uint32_t cardbus_cis_pointer;
        uint16_t subsystem_vendor_id;
        uint16_t subsystem_id;
        uint32_t expansion_rom_base_address;
        uint8_t capabilities_pointer;
        uint8_t _reserved[7];
        uint8_t interrupt_line;
        uint8_t interrupt_pin;
        uint8_t min_grant;
        uint8_t max_latency;
    };
} pci_dev;

#define PCI_CONFIG_ADDRESS  0xCF8
#define PCI_CONFIG_DATA 0xCFC

uint32_t get_register(pci_in_data data);
void get_device_info(pci_in_data data, pci_dev* info);
pci_dev* get_device(uint8_t index);
void pci_init();
uint8_t get_number_of_devices();

#endif