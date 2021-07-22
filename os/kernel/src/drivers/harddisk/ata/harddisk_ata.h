#ifndef HARDDISK_ATA_H
#define HARDDISK_ATA_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "assembly/io.h"
#include "drivers/harddisk/harddisk_header.h"
#include "harddisk_ata_command_matrix.h"

//! Reads hard disk sector.
/*!
    \param type Type of harddisk.
    \param bus Type of bus.
    \param high_lba Higher bits of lba (24 bits are used).
    \param low_lba Lower bits of lba (24 bits are used).
    \param buffer Buffer of 256 16-bits values for store data.
    \return 1 = success, -1 = disk error, -2 = parameter error.
*/
int8_t __harddisk_ata_read_sector(HARDDISK_ATA_MASTER_SLAVE type, HARDDISK_ATA_BUS_TYPE bus, uint32_t high_lba, uint32_t low_lba, uint16_t *buffer);

//! Write hard disk sector.
/*!
    \param type Type of harddisk.
    \param bus Type of bus.
    \param high_lba Higher bits of lba (24 bits are used).
    \param low_lba Lower bits of lba (24 bits are used).
    \param buffer Buffer of 256 16-bits values for store data.
    \return 1 = success, -1 = disk error, -2 = parameter error.
*/
int8_t __harddisk_ata_write_sector(HARDDISK_ATA_MASTER_SLAVE type, HARDDISK_ATA_BUS_TYPE bus, uint32_t high_lba, uint32_t low_lba, uint16_t *buffer);

//! Get user addressable sectors of hard disk.
/*!
    \param data Pointer to identify data of device.
    \return Number of user addressable sectors.
*/
uint32_t __harddisk_ata_get_user_addressable_sectors(const harddisk_identify_device_data *data);

//! Get space of hard disk.
/*!
    \param data Pointer to identify data of device.
    \return Space in bytes.
*/
uint32_t __harddisk_ata_get_disk_space(const harddisk_identify_device_data *data);

//! Doing pooling.
/*!
    Check if disk is ready.
    \param port Port for do delay.
    \return 1 if ready, -1 if error
*/
int8_t __harddisk_ata_poll(uint16_t port);

#endif