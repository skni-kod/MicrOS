#ifndef HDD_WRAPPER_H
#define HDD_WRAPPER_H

#include "drivers/harddisk/harddisk.h"
#include "cpu/timer/timer.h"
#include "logger/logger.h"

int hdd_wrapper_get_device_number(HARDDISK_ATA_MASTER_SLAVE type, HARDDISK_ATA_BUS_TYPE bus);
int hdd_wrapper_get_type_by_device_number(int device);
int hdd_wrapper_get_bus_by_device_number(int device);

uint8_t *hdd_wrapper_read_sector(int device_number, int sector);
void hdd_wrapper_write_sector(int device_number, int sector, uint8_t *content);

#endif