#ifndef HARDDISK_HEADER_H
#define HARDDISK_HEADER_H

#include <stdint.h>
#include <stddef.h>
#include "assembly/io.h"
#include "harddisk_identify_device_data.h"
#include "harddisk_pio_mode_header.h"

//! Enum represent bus type.
typedef enum HARDDISK_ATA_BUS_TYPE
{ 
    HARDDISK_ATA_PRIMARY_BUS = 1, /*!< Primary bus. */
    HARDDISK_ATA_SECONDARY_BUS = 2 /*!< Secondary bus. */
} HARDDISK_ATA_BUS_TYPE;

//! Enum represent hard disk type.
typedef enum HARDDISK_ATA_MASTER_SLAVE
{
    HARDDISK_ATA_MASTER = 1, /*!< Master hard disk. */
    HARDDISK_ATA_SLAVE = 2 /*!< Slave hard disk. */
} HARDDISK_ATA_MASTER_SLAVE;

//! Enum represent hard disk status.
typedef enum HARDDISK_STATE
{
    HARDDISK_ERROR = -1, /*!< Error. */
    HARDDISK_NOT_PRESENT = 0, /*!< Not present. */
    HARDDISK_ATA_PRESENT = 1, /*!< ATA device present. */
    HARDDISK_ATAPI_PRESENT = 2, /*!< ATAPI device present. */
    HARDDISK_SATA_PRESENT = 3, /*!< SATA device present. */
    HARDDISK_SATAPI_PRESENT = 4 /*!< SATAPI device present. */
} HARDDISK_STATE;

//! Defines status of hard disks in system.
typedef struct harddisk_states
{
    //! Presence of primary master
    HARDDISK_STATE primary_master;
    //! Data of primary master
    harddisk_identify_device_data primary_master_data;
    //! Presence of primary slave
    HARDDISK_STATE primary_slave;
    //! Data of primary slave
    harddisk_identify_device_data primary_slave_data;
    //! Presence of secondary master
    HARDDISK_STATE secondary_master;
    //! Data of secondary master
    harddisk_identify_device_data secondary_master_data;
    //! Presence of secondary slave
    HARDDISK_STATE secondary_slave;
    //! Data of secondary slave
    harddisk_identify_device_data secondary_slave_data;
} harddisk_states;

//! Perform soft reset for given control_port
/*! Soft reset should set bit SRST in Device Control Register for 5 us.
    \param control_port Control port to perform reset
*/
void __harddisk_soft_reset_port(uint16_t control_port);

//! Makes 400ns delay.
/*!
    Makes 400ns delay by checking drive status 15 times. User after switching type of drive in bus.
    \param port Port for do delay.
*/
void __harddisk_400ns_delay(uint16_t port);

//! Gets pointers to hard disk data.
/*!
    Sets state to proper state and data from harddisk_states structure to proper values for given harddisk and bus type.
    \param type Type of harddisk.
    \param bus Type of bus.
    \param state Pointer to state.
    \param bus Pointer to data.
*/
void __harddisk_get_pointers(HARDDISK_ATA_MASTER_SLAVE type, HARDDISK_ATA_BUS_TYPE bus, const HARDDISK_STATE **state, const harddisk_identify_device_data **data);

#endif