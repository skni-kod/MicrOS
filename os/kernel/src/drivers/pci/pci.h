#ifndef PCI_H
#define PCI_H

#include <stdint.h>
#include "pci_device.h"
#include "pci_in_data.h"

#define PCI_DEVICE_COUNT 32

#define PCI_CONFIG_ADDRESS 0xCF8
#define PCI_CONFIG_DATA 0xCFC

void pci_init();
void pci_get_device_info(pci_in_data *data, pci_device *info);
pci_device *pci_get_device(uint8_t index);
uint8_t pci_get_number_of_devices();
void pci_insert_device(pci_device *dev, pci_in_data *data);
void pci_check_bridge(pci_device *dev);
void pci_check_device(uint16_t bus, uint16_t dev);
void pci_check_bus(uint16_t bus);
void pci_check_all_buses();

void pci_io_out(pci_device *dev, uint8_t desc_index, uint32_t value);
uint32_t pci_io_in(pci_device *dev, uint8_t desc_index);

#endif