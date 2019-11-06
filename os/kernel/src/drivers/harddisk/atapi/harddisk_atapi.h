#ifndef HARDDISK_ATAPI_H
#define HARDDISK_ATAPI_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "assembly/io.h"
#include "drivers/harddisk/harddisk_header.h"
#include "drivers/harddisk/harddisk_pio_mode_header.h"
#include "drivers/harddisk/harddisk_identify_device_data.h"

//! Check hard disk presence in computer.
/*!
    Check if given hard disk is installed in computer.
    \param type Type of harddisk.
    \param bus Type of bus.
    \param data Harddisk identify device data for current drive.
    \return 0 no disk, 1 disk present, -1 disk present but ERR set, -2 wrong parameters.
*/
int8_t __harddisk_atapi_check_presence(HARDDISK_ATA_MASTER_SLAVE type, HARDDISK_ATA_BUS_TYPE bus, harddisk_identify_device_data *data);

#endif