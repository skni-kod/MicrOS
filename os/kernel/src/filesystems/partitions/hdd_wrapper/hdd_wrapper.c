#include "hdd_wrapper.h"

uint8_t read_content[512];

int hdd_wrapper_get_device_number(HARDDISK_ATA_MASTER_SLAVE type, HARDDISK_ATA_BUS_TYPE bus)
{
    if (type == HARDDISK_ATA_MASTER && bus == HARDDISK_ATA_PRIMARY_BUS) return 0;
    if (type == HARDDISK_ATA_MASTER && bus == HARDDISK_ATA_SECONDARY_BUS) return 1;
    if (type == HARDDISK_ATA_SLAVE && bus == HARDDISK_ATA_PRIMARY_BUS) return 2;
    if (type == HARDDISK_ATA_SLAVE && bus == HARDDISK_ATA_SECONDARY_BUS) return 3;
    
    return -1;
}

int hdd_wrapper_get_type_by_device_number(int device)
{
    if (device == 0 || device == 1) return HARDDISK_ATA_MASTER;
    else return HARDDISK_ATA_SLAVE;
}

int hdd_wrapper_get_bus_by_device_number(int device)
{
    if (device == 0 || device == 2) return HARDDISK_ATA_PRIMARY_BUS;
    else return HARDDISK_ATA_SECONDARY_BUS;
}

uint8_t *hdd_wrapper_read_sector(int device_number, int sector)
{
    HARDDISK_ATA_MASTER_SLAVE type = hdd_wrapper_get_type_by_device_number(device_number);
    HARDDISK_ATA_BUS_TYPE bus = hdd_wrapper_get_bus_by_device_number(device_number);
    
    int8_t result = harddisk_read_sector(type, bus, 0, sector, (uint16_t *)read_content);
    switch (result)
    {
        case 0: logger_log_error("harddisk_read_sector returned 0 - non-present HDD"); break;
        case -1: logger_log_error("harddisk_read_sector returned -1 - disk error"); break;
        case -2: logger_log_error("harddisk_read_sector returned -2 - parameters error"); break;
    }
    
    return read_content;
}

void hdd_wrapper_write_sector(int device_number, int sector, uint8_t *content)
{
    HARDDISK_ATA_MASTER_SLAVE type = hdd_wrapper_get_type_by_device_number(device_number);
    HARDDISK_ATA_BUS_TYPE bus = hdd_wrapper_get_bus_by_device_number(device_number);
    
    int8_t result = harddisk_write_sector(type, bus, 0, sector, (uint16_t *)content);
    switch (result)
    {
        case 0: logger_log_error("harddisk_read_sector returned 0 - non-present HDD"); break;
        case -1: logger_log_error("harddisk_read_sector returned -1 - disk error"); break;
        case -2: logger_log_error("harddisk_read_sector returned -2 - parameters error"); break;
    }
    
    //uint32_t current_time = timer_get_system_clock();
    //while(timer_get_system_clock() - current_time < 15);
}