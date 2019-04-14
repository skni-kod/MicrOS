#ifndef PCI_IN_DATA
#define PCI_IN_DATA

typedef union pci_in_data {
    uint32_t bits;
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
} __attribute__((packed)) pci_in_data;

#endif