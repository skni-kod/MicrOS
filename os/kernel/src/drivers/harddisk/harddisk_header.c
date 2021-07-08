#include "harddisk_header.h"

//! Current states of all hard drives.
harddisk_states harddisk_current_states;

void __harddisk_400ns_delay(uint16_t port)
{
    for(int i = 0; i < 15; ++i)
    {
        io_in_byte(port + HARDDISK_CONTROL_ALTERNATE_STATUS_REGISTER_OFFSET);
    }
}

void __harddisk_soft_reset_port(uint16_t control_port)
{
    harddisk_control_device_control_register message = {.value = 0};
    message.fields.software_reset = 1;
    io_out_byte(control_port, message.value);
    // Wait 5 us
    for(int i = 0; i < 50000; ++i);
    message.fields.software_reset = 0;
    io_out_byte(control_port, message.value);
}

void __harddisk_get_pointers(HARDDISK_ATA_MASTER_SLAVE type, HARDDISK_ATA_BUS_TYPE bus, const HARDDISK_STATE **state, const harddisk_identify_device_data **data)
{
    *state = NULL;
    *data = NULL;
    switch(type)
    {
    case HARDDISK_ATA_MASTER:
        switch(bus)
        {
            case HARDDISK_ATA_PRIMARY_BUS:
                // Primary master
                *state = &harddisk_current_states.primary_master;
                *data = &harddisk_current_states.primary_master_data;
                break;
            case HARDDISK_ATA_SECONDARY_BUS:
                // Secondary master
                *state = &harddisk_current_states.secondary_master;
                *data = &harddisk_current_states.secondary_master_data;
                break;
            default:
                return;
        }
        break;
    case HARDDISK_ATA_SLAVE:
        switch(bus)
        {
            case HARDDISK_ATA_PRIMARY_BUS:
                // Primary slave
                *state = &harddisk_current_states.primary_slave;
                *data = &harddisk_current_states.primary_slave_data;
                break;
            case HARDDISK_ATA_SECONDARY_BUS:
                // Secondary slave
                *state = &harddisk_current_states.secondary_slave;
                *data = &harddisk_current_states.secondary_slave_data;
                break;
            default:
                return;
        }
        break;
    default:
        return;      
    }
}