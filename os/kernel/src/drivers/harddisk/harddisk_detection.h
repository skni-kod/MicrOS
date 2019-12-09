#ifndef HARDDISK_DETECTION_H
#define HARDDISK_DETECTION_H


#include "assembly/io.h"
#include "harddisk_header.h"
#include "ata/harddisk_ata.h"
#include "atapi/harddisk_atapi.h"

//! Check hard disk presence in computer.
/*!
    Check if given hard disk is installed in computer.
    \param type Type of harddisk.
    \param bus Type of bus.
    \param states Pointer to struct where to put state of hard disk.
    \return 0 no disk, 1 disk present, -1 disk present but ERR set, -2 wrong parameters.
*/
int8_t __harddisk_check_presence(HARDDISK_ATA_MASTER_SLAVE type, HARDDISK_ATA_BUS_TYPE bus, harddisk_states *states);

//! Check hard disk presence in computer.
/*!
    Check if given hard disk is installed in computer.
    \param type Type of harddisk.
    \param bus Type of bus.
    \param data Harddisk identify device data for current drive.
    \return 0 no disk, 1 disk present, -1 disk present but ERR set, -2 wrong parameters.
*/
int8_t __harddisk_get_identify_data(HARDDISK_ATA_MASTER_SLAVE type, HARDDISK_ATA_BUS_TYPE bus, harddisk_identify_device_data *data, uint8_t identify_command);

#endif