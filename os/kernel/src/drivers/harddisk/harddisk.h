#ifndef HARDDISK_H
#define HARDDISK_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "harddisk_pio_mode_header.h"
#include "harddisk_identify_devide_data.h"
#include "assembly/io.h"

//! Enum represent bus type.
typedef enum HARDDISK_BUS_TYPE
{
    HARDDISK_PRIMARY_BUS = 1, /*!< Primary bus. */
    HARDDISK_SECONDARY_BUS = 2 /*!< Secondary bus. */
} HARDDISK_BUS_TYPE;

//! Enum represent hard disk type.
typedef enum HARDDISK_MASTER_SLAVE
{
    HARDDISK_MASTER = 1, /*!< Master hard disk. */
    HARDDISK_SLAVE = 0 /*!< Slave hard disk. */
} HARDDISK_MASTER_SLAVE;

//! Enum represent hard disk status.
typedef enum HARDDISK_STATE
{
    HARDDISK_PRESENT = 1, /*!< Present. */
    HARDDISK_NOT_PRESENT = 0, /*!< Not present. */
    HARDDISK_ERROR = -1 /*!< Error. */
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
HARDDISK_STATE harddisk_get_state(HARDDISK_MASTER_SLAVE type, HARDDISK_BUS_TYPE bus);

//! Get serial number of hard disk.
/*!
    Function copy serial number to buffer and terminate string with /0.
    \param type Type of harddisk.
    \param bus Type of bus.
    \param buffer Buffer for serial number with size at least 21.
    \return Pointer to buffer where serial number is stored.
*/
char* harddisk_get_disk_serial_number_terminated(HARDDISK_MASTER_SLAVE type, HARDDISK_BUS_TYPE bus, char *buffer);

//! Get firmware version of hard disk.
/*!
    Function copy firmware version to buffer and terminate string with /0.
    \param type Type of harddisk.
    \param bus Type of bus.
    \param buffer Buffer firmware version with size at least 9.
    \return Pointer to buffer where firmware version is stored.
*/
char* harddisk_get_disk_firmware_version_terminated(HARDDISK_MASTER_SLAVE type, HARDDISK_BUS_TYPE bus, char *buffer);


//! Get model number of hard disk.
/*!
    Function copy model number to buffer and terminate string with /0.
    \param type Type of harddisk.
    \param bus Type of bus.
    \param buffer Buffer model number with size at least 41.
    \return Pointer to buffer where model number is stored.
*/
char* harddisk_get_disk_model_number_terminated(HARDDISK_MASTER_SLAVE type, HARDDISK_BUS_TYPE bus, char *buffer);

//! Get user addressable sectors of hard disk.
/*!
    \param type Type of harddisk.
    \param bus Type of bus.
    \return Number of user addressable sectors.
*/
uint32_t harddisk_get_user_addressable_sectors(HARDDISK_MASTER_SLAVE type, HARDDISK_BUS_TYPE bus);

//! Get space of hard disk.
/*!
    \param type Type of harddisk.
    \param bus Type of bus.
    \return Space in bytes.
*/
uint32_t harddisk_get_disk_space(HARDDISK_MASTER_SLAVE type, HARDDISK_BUS_TYPE bus);


//! Reads hard disk sector.
/*!
    \param type Type of harddisk.
    \param bus Type of bus.
    \param high_lba Higher bits of lba (24 bits are used).
    \param low_lba Lower bits of lba (24 bits are used).
    \param buffer Buffer f 256 16-bits values for store data.
    \return 1 = success, -1 = disk error, -2 = parameter error.
*/
uint8_t harddisk_read_sector(HARDDISK_MASTER_SLAVE type, HARDDISK_BUS_TYPE bus, uint32_t high_lba, uint32_t low_lba, uint16_t *buffer);

//! Initialization of hard disk driver.
/*!
    Should be called during boot up once. Check for hard disks and store informations about them.
*/
void harddisk_init();

//! Gets pointers to harddisk data.
/*!
    Sets state to proper state and data from harddisk_states structure to proper values for given harddisk and bus type.
    \param type Type of harddisk.
    \param bus Type of bus.
    \param state Pointer to state.
    \param bus Pointer to data.
*/
void harddisk_get_pointers(HARDDISK_MASTER_SLAVE type, HARDDISK_BUS_TYPE bus, HARDDISK_STATE **state, harddisk_identify_device_data **data);

//! Check hard disk presence in computer.
/*!
    Check if given hard disk is installed in computer.
    \param type Type of harddisk.
    \param bus Type of bus.
    \return 0 no disk, 1 disk present, -1 disk present but ERR set, -2 wrong parameters.
*/
uint8_t harddisk_check_presence(HARDDISK_MASTER_SLAVE type, HARDDISK_BUS_TYPE bus);

//! Doing pooling.
/*!
    Check if disk is ready.
    \param port Port for do delay.
    \return 1 if ready, -1 if error
*/
uint8_t harddisk_poll(uint16_t io_port);

//! Makes 400ns delay.
/*!
    Makes 400ns delay by checking drive status 4 times. User after switching type of drive in bus.
    \param port Port for do delay.
*/
void harddisk_400ns_delay(uint16_t port);

#endif