#ifndef HARDDISK_H
#define HARDDISK_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "harddisk_header.h"
#include "harddisk_detection.h"
#include "ata/harddisk_ata.h"
#include "atapi/harddisk_atapi.h"

//! Initialization of hard disk driver.
/*!
    Should be called during boot up once. Check for hard disks and store informations about them.
*/
void harddisk_init();

//! Get hard disk states.
/*!
    Call this function after init, which should be called during system boot up.
    \return State off all hard drives in system.
*/
harddisk_states harddisk_get_states();

//! Get status of hard disk.
/*!
    \param type Type of harddisk.
    \param bus Type of bus.
    \return State of hard disk.
*/
HARDDISK_STATE harddisk_get_state(HARDDISK_ATA_MASTER_SLAVE type, HARDDISK_ATA_BUS_TYPE bus);

//! Get identify data of hard disk.
/*!
    \param type Type of harddisk.
    \param bus Type of bus.
    \return Identify data of hard disk.
*/
const harddisk_identify_device_data* harddisk_get_identify_data(HARDDISK_ATA_MASTER_SLAVE type, HARDDISK_ATA_BUS_TYPE bus);

//! Get serial number of hard disk.
/*!
    Function copy serial number to buffer and terminate string with /0.
    \param type Type of harddisk.
    \param bus Type of bus.
    \param buffer Buffer for serial number with size at least 21.
    \return Pointer to buffer where serial number is stored.
*/
char* harddisk_get_disk_serial_number_terminated(HARDDISK_ATA_MASTER_SLAVE type, HARDDISK_ATA_BUS_TYPE bus, char *buffer);

//! Get firmware version of hard disk.
/*!
    Function copy firmware version to buffer and terminate string with /0.
    \param type Type of harddisk.
    \param bus Type of bus.
    \param buffer Buffer firmware version with size at least 9.
    \return Pointer to buffer where firmware version is stored.
*/
char* harddisk_get_disk_firmware_version_terminated(HARDDISK_ATA_MASTER_SLAVE type, HARDDISK_ATA_BUS_TYPE bus, char *buffer);

//! Get model number of hard disk.
/*!
    Function copy model number to buffer and terminate string with /0.
    \param type Type of harddisk.
    \param bus Type of bus.
    \param buffer Buffer model number with size at least 41.
    \return Pointer to buffer where model number is stored.
*/
char* harddisk_get_disk_model_number_terminated(HARDDISK_ATA_MASTER_SLAVE type, HARDDISK_ATA_BUS_TYPE bus, char *buffer);

//! Get user addressable sectors of hard disk.
/*!
    \param type Type of harddisk.
    \param bus Type of bus.
    \return Number of user addressable sectors.
*/
uint32_t harddisk_get_user_addressable_sectors(HARDDISK_ATA_MASTER_SLAVE type, HARDDISK_ATA_BUS_TYPE bus);

//! Get space of hard disk.
/*!
    \param type Type of harddisk.
    \param bus Type of bus.
    \return Space in bytes.
*/
uint32_t harddisk_get_disk_space(HARDDISK_ATA_MASTER_SLAVE type, HARDDISK_ATA_BUS_TYPE bus);

//! Check if device is removable media.
/*!
    Check if device can contain removable media.
    \param type Type of harddisk.
    \param bus Type of bus.
    \return True if device can contain removable media, false otherwise.
*/
bool harddisk_get_is_removable_media_device(HARDDISK_ATA_MASTER_SLAVE type, HARDDISK_ATA_BUS_TYPE bus);

//! Reads hard disk sector.
/*!
    \param type Type of harddisk.
    \param bus Type of bus.
    \param high_lba Higher bits of lba (24 bits are used).
    \param low_lba Lower bits of lba (24 bits are used).
    \param buffer Buffer of 256 16-bits values for store data.
    \return 1 = success, -1 = disk error, -2 = parameter error.
*/
int8_t harddisk_read_sector(HARDDISK_ATA_MASTER_SLAVE type, HARDDISK_ATA_BUS_TYPE bus, uint32_t high_lba, uint32_t low_lba, uint16_t *buffer);

//! Write hard disk sector.
/*!
    \param type Type of harddisk.
    \param bus Type of bus.
    \param high_lba Higher bits of lba (24 bits are used).
    \param low_lba Lower bits of lba (24 bits are used).
    \param buffer Buffer of 256 16-bits values for store data.
    \return 1 = success, -1 = disk error, -2 = parameter error.
*/
int8_t harddisk_write_sector(HARDDISK_ATA_MASTER_SLAVE type, HARDDISK_ATA_BUS_TYPE bus, uint32_t high_lba, uint32_t low_lba, uint16_t *buffer);

#endif