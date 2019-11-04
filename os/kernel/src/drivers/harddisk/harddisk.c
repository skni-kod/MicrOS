#include "harddisk.h"

harddisk_states current_states;

harddisk_states harddisk_get_states()
{
    return current_states;
}

char* harddisk_get_disk_serial_number_terminated(MASTER_SLAVE type, BUS_TYPE bus, char *buffer)
{
    HARDDISK_STATE *state;
    harddisk_identify_device_data *data;
    harddisk_get_pointers(type, bus, &state, &data);

    // Copy serial number to buffer
    for(int i = 0; i < HARDDISK_SERIAL_NUMBER_LENGTH; i += 2)
    {
        buffer[i] = data->fields.serial_number[i + 1];
        buffer[i + 1] = data->fields.serial_number[i];
    }
    buffer[HARDDISK_SERIAL_NUMBER_LENGTH] = '\0';
    return buffer;
}

char* harddisk_get_disk_firmware_version_terminated(MASTER_SLAVE type, BUS_TYPE bus, char *buffer)
{
    HARDDISK_STATE *state;
    harddisk_identify_device_data *data;
    harddisk_get_pointers(type, bus, &state, &data);

    // Copy firmware version to buffer
    for(int i = 0; i < HARDDISK_FIRMWARE_VERSION_LENGTH; i += 2)
    {
        buffer[i] = data->fields.firmware_version[i + 1];
        buffer[i + 1] = data->fields.firmware_version[i];
    }
    buffer[HARDDISK_FIRMWARE_VERSION_LENGTH] = '\0';
    return buffer;
}

char* harddisk_get_disk_model_number_terminated(MASTER_SLAVE type, BUS_TYPE bus, char *buffer)
{
    HARDDISK_STATE *state;
    harddisk_identify_device_data *data;
    harddisk_get_pointers(type, bus, &state, &data);

    // Copy model number to buffer
    for(int i = 0; i < HARDDISK_MODEL_NUMBER_LENGTH; i += 2)
    {
        buffer[i] = data->fields.model_number[i + 1];
        buffer[i + 1] = data->fields.model_number[i];
    }
    buffer[HARDDISK_MODEL_NUMBER_LENGTH] = '\0';
    return buffer;
}

uint32_t harddisk_get_disk_space(MASTER_SLAVE type, BUS_TYPE bus)
{
    HARDDISK_STATE *state;
    harddisk_identify_device_data *data;
    harddisk_get_pointers(type, bus, &state, &data);
    
    // Multiply total number of user addressable sectors by number of bytes per sector (currently hard coded).
    return data->fields.total_number_of_user_addressable_sectors * 512;
}

void harddisk_init()
{
    current_states.primary_master = (HARDDISK_STATE) harddisk_check_presence(HARDDISK_MASTER, HARDDISK_PRIMARY_BUS);
    current_states.primary_slave = (HARDDISK_STATE) harddisk_check_presence(HARDDISK_SLAVE, HARDDISK_PRIMARY_BUS);
    current_states.secondary_master = (HARDDISK_STATE) harddisk_check_presence(HARDDISK_MASTER, HARDDISK_SECONDARY_BUS);
    current_states.secondary_slave = (HARDDISK_STATE) harddisk_check_presence(HARDDISK_SLAVE, HARDDISK_SECONDARY_BUS);
}

void harddisk_get_pointers(MASTER_SLAVE type, BUS_TYPE bus, HARDDISK_STATE **state, harddisk_identify_device_data **data)
{
    *state = NULL;
    *data = NULL;
    switch(type)
    {
    case HARDDISK_MASTER:
        switch(bus)
        {
            case HARDDISK_PRIMARY_BUS:
                // Primary master
                *state = &current_states.primary_master;
                *data = &current_states.primary_master_data;
                break;
            case HARDDISK_SECONDARY_BUS:
                // Secondary master
                *state = &current_states.secondary_master;
                *data = &current_states.secondary_master_data;
                break;
            default:
                return;
        }
        break;
    case HARDDISK_SLAVE:
        switch(bus)
        {
            case HARDDISK_PRIMARY_BUS:
                // Primary slave
                *state = &current_states.primary_slave;
                *data = &current_states.primary_slave_data;
                break;
            case HARDDISK_SECONDARY_BUS:
                // Secondary slave
                *state = &current_states.secondary_slave;
                *data = &current_states.secondary_slave_data;
                break;
            default:
                return;
        }
        break;
    default:
        return;      
    }
}

uint8_t harddisk_check_presence(MASTER_SLAVE type, BUS_TYPE bus)
{
    uint16_t io_port = 0;
    uint16_t message_to_drive = 0;

    // Set port of drive
    if (bus == HARDDISK_PRIMARY_BUS)
    {
        io_port = harddisk_primary_bus_io_port;
    }
    else if(bus == HARDDISK_SECONDARY_BUS)
    {
        io_port = harddisk_secondary_bus_io_port;
    }
    else
    {
        return -2;
    }

    // Set drive
    switch (type)
    {
    case HARDDISK_MASTER:
        message_to_drive = 0xA0;
        break;
    case HARDDISK_SLAVE:
        message_to_drive = 0xB0;
        break;
    default:
        return -2;
    }

    harddisk_identify_device_data *data;
    if(type == HARDDISK_MASTER && bus == HARDDISK_PRIMARY_BUS) data = &current_states.primary_master_data;
    else if(type == HARDDISK_SLAVE && bus == HARDDISK_PRIMARY_BUS) data = &current_states.primary_slave_data;
    else if(type == HARDDISK_MASTER && bus == HARDDISK_SECONDARY_BUS) data = &current_states.secondary_master_data;
    else if(type == HARDDISK_SLAVE && bus == HARDDISK_SECONDARY_BUS) data = &current_states.secondary_slave_data;

    // Send message to drive
    io_out_byte(io_port + harddisk_io_drive_head_register_offset, message_to_drive);

    // Read four times to allow drive to set
    io_in_byte(io_port + harddisk_io_drive_head_register_offset);
    io_in_byte(io_port + harddisk_io_drive_head_register_offset);
    io_in_byte(io_port + harddisk_io_drive_head_register_offset);
    io_in_byte(io_port + harddisk_io_drive_head_register_offset);

    // Set the Sectorcount, LBAlo, LBAmid, and LBAhi IO ports to 0
    io_out_word(io_port + harddisk_io_sector_count_register_offset, 0);
    io_out_word(io_port + harddisk_io_sector_number_register_offset, 0);
    io_out_word(io_port + harddisk_io_cylinder_low_register_offset, 0);
    io_out_word(io_port + harddisk_io_cylinder_high_register_offset, 0);

    // Send the IDENTIFY command (0xEC) to the Command IO port (0x1F7).
    io_out_byte(0x1F7, 0xEC);

    // Read the Status port (0x1F7) again.
    harddisk_io_control_status_register result;
    result.value = io_in_byte(0x1F7);

    // If the value read is 0, the drive does not exist.
    if(result.value == 0)
    {
        return 0;
    }  
    else
    {   // For any other value: poll the Status port (0x1F7) until bit 7 (BSY, value = 0x80) clears.
        for(;;)
        {
            if(result.fields.busy == 0)
            {
                // Otherwise, continue polling one of the Status ports until bit 3 (DRQ, value = 8) sets, or until bit 0 (ERR, value = 1) sets.
                for(;;)
                {
                    result.value = io_in_byte(0x1F7);
                    if(result.fields.has_pio_data_to_transfer_or_ready_to_accept_pio_data == 1)
                    {
                        for(int i = 0; i < 256; i++)
                        {
                            //  Read 256 16-bit values, and store them.
                            data->values[i] = io_in_word(io_port);
                        }
                        return 1;
                    }
                    else if(result.fields.error_occurred)
                    {
                        return -1;
                    }
                }
                return true;
            }
            else
            {
                // Because of some ATAPI drives that do not follow spec, at this point you need to check the LBAmid and LBAhi ports (0x1F4 and 0x1F5)
                // to see if they are non-zero. If so, the drive is not ATA, and you should stop polling.
                if(io_in_word(io_port + harddisk_io_cylinder_low_register_offset) != 0 || io_in_word(io_port + harddisk_io_cylinder_high_register_offset) != 0)
                {
                    return 0;
                }
            }
            result.value = io_in_byte(0x1F7);
        }
    }
}