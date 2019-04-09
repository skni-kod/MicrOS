#ifndef PCI_DEVICE
#define PCI_DEVICE

typedef union pci_device {
    uint8_t bits_8[64];
    uint16_t bits_16[32];
    uint32_t bits_32[16];
    struct
    {
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
} __attribute__((packed)) pci_device;

#endif