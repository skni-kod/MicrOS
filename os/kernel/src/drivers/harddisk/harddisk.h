#ifndef HARDDISK_H
#define HARDDISK_H

#include <stdint.h>
#include <stdbool.h>
#include "harddisk_pio_mode_header.h"
#include "assembly/io.h"

//! Enum represent bus type.
typedef enum BUS_TYPE
{
    //! Primary bus.
    HARDDISK_PRIMARY_BUS = 1,
    //! Secondary bus.
    HARDDISK_SECONDARY_BUS = 2
} BUS_TYPE;

//! Enum represent hard disk type.
typedef enum MASTER_SLAVE
{
    //! Master hard disk.
    HARDDISK_MASTER = 1,
    //! Slave hard disk.
    HARDDISK_SLAVE = 0
} MASTER_SLAVE;

//! Enum represent hard disk status.
typedef enum HARDDISK_STATE
{
    //! Present.
    PRESENT = 1,
    //! Not present.
    NOT_PRESENT = 0,
    //! Error.
    ERROR = -1
} HARDDISK_STATE;

//! Defines status of hard disks in system.
typedef struct harddisk_states
{
    //! Presence od primary master
    HARDDISK_STATE primary_master;
    //! Presence od primary slave
    HARDDISK_STATE primary_slave;
    //! Presence od secondary master
    HARDDISK_STATE secondary_master;
    //! Presence od secondary slave
    HARDDISK_STATE secondary_slave;
} harddisk_states;

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
harddisk_states get_harddisk_states();

//! Check hard disk presence in computer.
/*!
    Check if given hard disk is installed in computer.
    \param master Set 1 to check master, set 2 to check slave.
    \param bus Set 1 to check primary bus, set 2 to check secondary bus.
    \return 0 no disk, 1 disk present, -1 disk present but ERR set, -2 wrong parameters.
*/
uint8_t check_harddisk_presence(MASTER_SLAVE master, BUS_TYPE bus);

#endif