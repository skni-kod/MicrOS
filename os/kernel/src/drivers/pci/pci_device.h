#ifndef PCI_DEVICE
#define PCI_DEVICE

/**
 * Union with inner structure representing PCI configuration space.
 * Stores all data from configuration space.
 * Divided into 64 byte predefined header with first 16 bytes always the same.
 * Further 192 bytes are device dependent and driver needs to work with these.
 */
typedef union pci_config
{
    uint8_t bytes[256];
    uint16_t words[128];
    uint32_t dwords[64];
    struct
    {
        //ALWAYS THE SAME
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
        //DEPENDS ON HEADER TYPE
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
        union device_dependent
        {
            uint8_t bytes[192];
            uint16_t words[96];
            uint32_t dwords[48];
        } device_dependent;
    };
} __attribute__((packed)) pci_config;

/**
 * Structure representing device. Holds basic information about device including bus and device index and its configuration space.
 */
typedef struct pci_device
{
    uint16_t bus_id;
    uint16_t dev_id;
    uint8_t func_id;
    pci_config config;
} pci_device;

typedef union pci_cmd_address_reg
{
    struct
    {
        uint32_t always_zero : 2;
        uint32_t register_num : 6;
        uint32_t function_num : 3;
        uint32_t device_num : 5;
        uint32_t bus_num : 8;
        uint32_t reserved : 7;
        uint32_t enable : 1;
    };
    uint32_t bits;
} __attribute__((packed)) pci_cmd_address_reg;

#endif