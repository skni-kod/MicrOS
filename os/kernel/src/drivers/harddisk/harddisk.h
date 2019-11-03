#ifndef HARDDISK_H
#define HARDDISK_H

#include <stdint.h>
#include <stdbool.h>
#include "harddisk_pio_mode_header.h"
#include "assembly/io.h"

//! Check hard disk presence in computer.
/*!
    Check if given hard disk is installed in computer.
    \param master Set 1 to check master, set 0 to check slave.
    \param bus Set 1 to check primary bus, set 2 to check secondary bus.
    \return Existence of disk.
*/
bool check_harddisk_presence(uint8_t master, uint8_t bus);

#endif HARDDISK_H