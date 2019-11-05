#include "harddisk.h"

harddisk_states current_states;

harddisk_states harddisk_get_states()
{
    return current_states;
}

HARDDISK_STATE harddisk_get_state(HARDDISK_MASTER_SLAVE type, HARDDISK_BUS_TYPE bus)
{
    HARDDISK_STATE *state;
    harddisk_identify_device_data *data;
    harddisk_get_pointers(type, bus, &state, &data);

    return *state;
}

char* harddisk_get_disk_serial_number_terminated(HARDDISK_MASTER_SLAVE type, HARDDISK_BUS_TYPE bus, char *buffer)
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

char* harddisk_get_disk_firmware_version_terminated(HARDDISK_MASTER_SLAVE type, HARDDISK_BUS_TYPE bus, char *buffer)
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

char* harddisk_get_disk_model_number_terminated(HARDDISK_MASTER_SLAVE type, HARDDISK_BUS_TYPE bus, char *buffer)
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

uint32_t harddisk_get_user_addressable_sectors(HARDDISK_MASTER_SLAVE type, HARDDISK_BUS_TYPE bus)
{
    HARDDISK_STATE *state;
    harddisk_identify_device_data *data;
    harddisk_get_pointers(type, bus, &state, &data);
    
    return data->fields.total_number_of_user_addressable_sectors;
}

uint32_t harddisk_get_disk_space(HARDDISK_MASTER_SLAVE type, HARDDISK_BUS_TYPE bus)
{
    HARDDISK_STATE *state;
    harddisk_identify_device_data *data;
    harddisk_get_pointers(type, bus, &state, &data);
    
    // Multiply total number of user addressable sectors by number of bytes per sector (currently hard coded).
    return data->fields.total_number_of_user_addressable_sectors * 512;
}

uint16_t* harddisk_read_sector(HARDDISK_MASTER_SLAVE type, HARDDISK_BUS_TYPE bus, uint32_t high_lba, uint32_t low_lba, uint16_t *buffer)
{
    if(buffer == NULL) return;
    uint16_t io_port = 0;
    uint16_t message_to_drive = 0;

    // Set port of drive
    if (bus == HARDDISK_PRIMARY_BUS)
    {
        io_port = HARDDISK_PRIMARY_BUS_IO_PORT;
    }
    else if(bus == HARDDISK_SECONDARY_BUS)
    {
        io_port = HARDDISK_SECONDARY_BUS_IO_PORT;
    }
    else
    {
        return -2;
    }

    // Set drive
    switch (type)
    {
    case HARDDISK_MASTER:
        message_to_drive = 0x40;
        break;
    case HARDDISK_SLAVE:
        message_to_drive = 0x50;
        break;
    default:
        return -2;
    }

    // Send message to drive
    io_out_byte(io_port + HARDDISK_IO_DRIVE_HEAD_REGISTER_OFFSET, message_to_drive);

    // Send what to read
    io_out_byte(io_port + HARDDISK_IO_SECTOR_COUNT_REGISTER_OFFSET, 0);
    io_out_byte(io_port + HARDDISK_IO_SECTOR_NUMBER_REGISTER_OFFSET, (uint8_t)high_lba);
    io_out_byte(io_port + HARDDISK_IO_CYLINDER_LOW_REGISTER_OFFSET, (uint8_t)(high_lba >> 8));
    io_out_byte(io_port + HARDDISK_IO_CYLINDER_HIGH_REGISTER_OFFSET, (uint8_t)(high_lba >> 16));
    io_out_byte(io_port + HARDDISK_IO_SECTOR_COUNT_REGISTER_OFFSET, 1);
    io_out_byte(io_port + HARDDISK_IO_SECTOR_NUMBER_REGISTER_OFFSET, (uint8_t)low_lba);
    io_out_byte(io_port + HARDDISK_IO_CYLINDER_LOW_REGISTER_OFFSET, (uint8_t)(low_lba >> 8));
    io_out_byte(io_port + HARDDISK_IO_CYLINDER_HIGH_REGISTER_OFFSET, (uint8_t)(low_lba >> 16));

    // Send the "READ SECTORS EXT" command (0x24) to port 0x1F7: outb(0x1F7, 0x24)
    io_out_byte(io_port + HARDDISK_IO_COMMAND_REGISTER_OFFSET, 0x24);

    harddisk_400ns_delay(io_port);

    // For any other value: poll the Status port until bit 7 (BSY, value = 0x80) clears.
    harddisk_io_control_status_register result;
    for(;;)
    {
        result.value = io_in_byte(io_port + HARDDISK_IO_STATUS_REGISTER_OFFSET);
        if(result.fields.busy == 0)
        {
            // Otherwise, continue polling one of the Status ports until bit 3 (DRQ, value = 8) sets, or until bit 0 (ERR, value = 1) sets.
            for(;;)
            {
                result.value = io_in_byte(io_port + HARDDISK_IO_STATUS_REGISTER_OFFSET);
                if(result.fields.has_pio_data_to_transfer_or_ready_to_accept_pio_data == 1)
                {
                    for(int i = 0; i < 256; i++)
                    {
                        //  Read 256 16-bit values, and store them.
                        buffer[i] = io_in_word(io_port);
                    }
                    return 1;
                }
                else if(result.fields.error_occurred == 1)
                {
                    return;
                }
            }
        }
 
    }

}

void harddisk_init()
{
    current_states.primary_master = (HARDDISK_STATE) harddisk_check_presence(HARDDISK_MASTER, HARDDISK_PRIMARY_BUS);
    current_states.primary_slave = (HARDDISK_STATE) harddisk_check_presence(HARDDISK_SLAVE, HARDDISK_PRIMARY_BUS);
    current_states.secondary_master = (HARDDISK_STATE) harddisk_check_presence(HARDDISK_MASTER, HARDDISK_SECONDARY_BUS);
    current_states.secondary_slave = (HARDDISK_STATE) harddisk_check_presence(HARDDISK_SLAVE, HARDDISK_SECONDARY_BUS);
}

void harddisk_get_pointers(HARDDISK_MASTER_SLAVE type, HARDDISK_BUS_TYPE bus, HARDDISK_STATE **state, harddisk_identify_device_data **data)
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

uint8_t harddisk_check_presence(HARDDISK_MASTER_SLAVE type, HARDDISK_BUS_TYPE bus)
{
    uint16_t io_port = 0;
    uint16_t message_to_drive = 0;

    // Set port of drive
    if (bus == HARDDISK_PRIMARY_BUS)
    {
        io_port = HARDDISK_PRIMARY_BUS_IO_PORT;
    }
    else if(bus == HARDDISK_SECONDARY_BUS)
    {
        io_port = HARDDISK_SECONDARY_BUS_IO_PORT;
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
    io_out_byte(io_port + HARDDISK_IO_DRIVE_HEAD_REGISTER_OFFSET, message_to_drive);

    // Make 400ns delay
    harddisk_400ns_delay(io_port);

    // Set the Sectorcount, LBAlo, LBAmid, and LBAhi IO ports to 0
    io_out_word(io_port + HARDDISK_IO_SECTOR_COUNT_REGISTER_OFFSET, 0);
    io_out_word(io_port + HARDDISK_IO_SECTOR_NUMBER_REGISTER_OFFSET, 0);
    io_out_word(io_port + HARDDISK_IO_CYLINDER_LOW_REGISTER_OFFSET, 0);
    io_out_word(io_port + HARDDISK_IO_CYLINDER_HIGH_REGISTER_OFFSET, 0);

    // Send the IDENTIFY command (0xEC) to the Command IO port.
    io_out_byte(io_port + HARDDISK_IO_COMMAND_REGISTER_OFFSET, IDENTIFY_COMMAND);

    // Read the Status port again.
    harddisk_io_control_status_register result;
    result.value = io_in_byte(io_port + HARDDISK_IO_STATUS_REGISTER_OFFSET);

    // If the value read is 0, the drive does not exist.
    if(result.value == 0)
    {
        return 0;
    }  
    else
    {   // For any other value: poll the Status port until bit 7 (BSY, value = 0x80) clears.
        for(;;)
        {
            if(result.fields.busy == 0)
            {
                // Otherwise, continue polling one of the Status ports until bit 3 (DRQ, value = 8) sets, or until bit 0 (ERR, value = 1) sets.
                for(;;)
                {
                    result.value = io_in_byte(io_port + HARDDISK_IO_STATUS_REGISTER_OFFSET);
                    if(result.fields.has_pio_data_to_transfer_or_ready_to_accept_pio_data == 1)
                    {
                        for(int i = 0; i < 256; i++)
                        {
                            //  Read 256 16-bit values, and store them.
                            data->values[i] = io_in_word(io_port);
                        }
                        return 1;
                    }
                    else if(result.fields.error_occurred == 1)
                    {
                        return -1;
                    }
                }
            }
            else
            {
                // Because of some ATAPI drives that do not follow spec, at this point you need to check the LBAmid and LBAhi ports (0x1F4 and 0x1F5)
                // to see if they are non-zero. If so, the drive is not ATA, and you should stop polling.
                if(io_in_word(io_port + HARDDISK_IO_CYLINDER_LOW_REGISTER_OFFSET) != 0 || io_in_word(io_port + HARDDISK_IO_CYLINDER_HIGH_REGISTER_OFFSET) != 0)
                {
                    return 0;
                }
            }
            result.value = io_in_byte(io_port + HARDDISK_IO_STATUS_REGISTER_OFFSET);
        }
    }
}

void harddisk_400ns_delay(uint16_t port)
{
    for(int i = 0; i < 4; ++i)
    {
        io_in_byte(port + HARDDISK_IO_STATUS_REGISTER_OFFSET);
    }
}