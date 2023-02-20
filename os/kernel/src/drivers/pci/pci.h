#ifndef PCI_H
#define PCI_H

/*****************************************************************************
 * If you need to change anything here, please first read PCI Specification! *
 * PCI devices have some limitations still, some things are described in     *
 * chipset specifications or PCI controller ones. Do not change things if    *
 * you're not sure what that thing does!                                     *
 *****************************************************************************/

#include <stdint.h>
#include <stdbool.h>
#include "pci_device.h"

#define PCI_DEVICE_COUNT 32

#define PCI_CONFIG_ADDRESS 0xCF8
#define PCI_CONFIG_DATA 0xCFC

//REMEMBER!
//These offsets are not register numbers, but byte offsets in whole PCI configuration space.
//If Command register is indeed 0x1 register then it's byte offset will be 0x4 according to PCI Specification.
#define PCI_COMMAND_REGISTER 0x4
#define PCI_COMMAND_BUSMASTER_FLAG (1 << 2)

/**
 * @brief Inits PCI driver and detect all PCI devices.
 */
void pci_init();

/**
 * @brief Gets device info based on @p data structure and returns device information via info structure.
 * @param data structure holding data to send proper command to PCI I/O space.
 * @param info structure holding returned data about PCI device.
 */
void pci_get_device_info(pci_cmd_address_reg *data, pci_device *info);

/**
 * @brief Gets specified device from list.
 * @param index id of PCI device we want to get.
 * @return Pointer to PCI device at specified index.
 */
pci_device *pci_get_device(uint8_t index);


/**
 * @brief Get number of detected PCI devices
 * @return Number of detected PCI devices
*/
uint8_t pci_get_number_of_devices();

/**
 * @brief Adds PCI device specified by @p dev into system PCI device table.
 * @param dev pointer to newly created PCI device that is going to be added to system table.
 * @param data pointer to structure describing command to PCI I/O space.
 */
void pci_insert_device(pci_device *dev, pci_cmd_address_reg *data);

/**
 * @brief Checks if specified device is PCI Bridge and looks for connected devices to the bridge.
 * @param dev id of the device to be checked.
 */
void pci_check_bridge(pci_device *dev);

/**
 * @brief Checks what kind of PCI device it is and adds it to system table.
 * @param bus id of bus at which device is preset.
 * @param dev id of the device at bus.
 */
void pci_check_device(uint16_t bus, uint16_t dev);

/**
 * @brief Checks specified bus for PCI devices
 * @param bus number of bus to be checked in range 0 - 7
 */
void pci_check_bus(uint16_t bus);

/**
 * @brief Checks all PCI buses available for PCI devices.
 */
void pci_check_all_buses();

/**
 * @brief Sets specified PCI Device as Bus Master.
 * @param device pointer to PCI device.
 * @param value if true PCI device is set as Bus Master.
 */
void pci_busmaster_set(pci_device *device, bool value);

/**
 * @brief Writes byte at offset from PCI device configuration space
 * @param dev pointer to device to which configuration will be written
 * @param offset offset in bytes
 */
void pci_config_out_byte(pci_device *dev, uint8_t offset, uint8_t value);
/**
 * @brief Writes word at offset from PCI device configuration space
 * @param dev pointer to device to which configuration will be written
 * @param offset offset in bytes
 */
void pci_config_out_word(pci_device *dev, uint8_t offset, uint16_t value);
/**
 * @brief Writes dword at offset from PCI device configuration space
 * @param dev pointer to device to which configuration will be written
 * @param offset offset in bytes
 */
void pci_config_out_dword(pci_device *dev, uint8_t offset, uint32_t value);

/**
 * @brief Reads byte at offset from PCI device configuration space
 * @param dev pointer to device from which configuration will be read
 * @param offset offset in bytes
 * @return read byte
 */
uint8_t pci_config_in_byte(pci_device *dev, uint8_t offset);
/**
 * @brief Reads word at offset from PCI device configuration space
 * @param dev pointer to device from which configuration will be read
 * @param offset offset in bytes
 * @return read word
 */
uint16_t pci_config_in_word(pci_device *dev, uint8_t offset);
/**
 * @brief Reads dword at offset from PCI device configuration space
 * @param dev pointer to device from which configuration will be read
 * @param offset offset in bytes
 * @return read dword
 */
uint32_t pci_config_in_dword(pci_device *dev, uint8_t offset);

/**
 * @brief Prints all PCI devices from system table
*/
void pci_print_devices();

#endif