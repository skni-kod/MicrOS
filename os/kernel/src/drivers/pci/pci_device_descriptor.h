#ifndef PCI_DEVICE_DESCRIPTOR
#define PCI_DEVICE_DESCRIPTOR

typedef union pci_device_descriptor
{
    uint32_t registers[16];
} __attribute__((packed)) pci_device_descriptor;

#endif