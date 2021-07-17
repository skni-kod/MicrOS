#include "harddisk_detection.h"

int8_t __harddisk_check_presence(HARDDISK_ATA_MASTER_SLAVE type, HARDDISK_ATA_BUS_TYPE bus, harddisk_states *states)
{
    uint16_t io_port = 0;
    uint16_t control_port = 0;
    harddisk_io_drive_head_register message_to_drive = {.value = 0};

    // Set port of drive
    if (bus == HARDDISK_ATA_PRIMARY_BUS) {io_port = HARDDISK_ATA_PRIMARY_BUS_IO_PORT; control_port = HARDDISK_ATA_PRIMARY_BUS_CONTROL_PORT;}
    else if(bus == HARDDISK_ATA_SECONDARY_BUS) {io_port = HARDDISK_ATA_SECONDARY_BUS_IO_PORT; control_port = HARDDISK_ATA_SECONDARY_BUS_CONTROL_PORT;}
    else return -2;

    // Set drive
    switch (type)
    {
    case HARDDISK_ATA_MASTER:
        // For master set it to 0xA0. We set 2 bits that should be always 1.
        message_to_drive.fields.always_set_field_1 = 1;
        message_to_drive.fields.always_set_field_2 = 1;
        break;
    case HARDDISK_ATA_SLAVE:
        // For slave set it to 0xB0. We set 2 bits that should be always 1 and drive number to 1.
        message_to_drive.fields.always_set_field_1 = 1;
        message_to_drive.fields.always_set_field_2 = 1;
        message_to_drive.fields.drive_number = 1;
        break;
    default:
        return -2;
    }

    harddisk_identify_device_data *data;
    if(type == HARDDISK_ATA_MASTER && bus == HARDDISK_ATA_PRIMARY_BUS) data = &states->primary_master_data;
    else if(type == HARDDISK_ATA_SLAVE && bus == HARDDISK_ATA_PRIMARY_BUS) data = &states->primary_slave_data;
    else if(type == HARDDISK_ATA_MASTER && bus == HARDDISK_ATA_SECONDARY_BUS) data = &states->secondary_master_data;
    else if(type == HARDDISK_ATA_SLAVE && bus == HARDDISK_ATA_SECONDARY_BUS) data = &states->secondary_slave_data;


    // Do soft reset
    __harddisk_soft_reset_port(control_port);


    // Send message to drive
    io_out_byte(io_port + HARDDISK_IO_DRIVE_HEAD_REGISTER_OFFSET, message_to_drive.value);


    // Make 400ns delay
    __harddisk_400ns_delay(control_port);


    uint8_t cylinder_low = io_in_byte(io_port +  HARDDISK_IO_CYLINDER_LOW_REGISTER_OFFSET);
    uint8_t cylinder_high = io_in_byte(io_port +  HARDDISK_IO_CYLINDER_HIGH_REGISTER_OFFSET);


    /* differentiate ATA, ATAPI, SATA and SATAPI */
    if (cylinder_low==0x3c && cylinder_high==0xc3)
    {
        return HARDDISK_SATA_PRESENT;
    }
    if (cylinder_low==0x69 && cylinder_high==0x96)
    {
        return HARDDISK_SATAPI_PRESENT;
    }
	if (cylinder_low==0x14 && cylinder_high==0xEB)
    {
        __harddisk_get_identify_data(type, bus, data, HARDDISK_IDENTIFY_PACKET_DEVICE);
        return HARDDISK_ATAPI_PRESENT;
    }
	if (cylinder_low==0 && cylinder_high == 0)
    {
        __harddisk_get_identify_data(type, bus, data, HARDDISK_IDENTIFY_DEVICE_COMMAND);
        return HARDDISK_ATA_PRESENT;
    }
	
	return HARDDISK_NOT_PRESENT;
}

int8_t __harddisk_get_identify_data(HARDDISK_ATA_MASTER_SLAVE type, HARDDISK_ATA_BUS_TYPE bus, harddisk_identify_device_data *data, uint8_t identify_command)
{
    uint16_t io_port = 0;
    uint16_t control_port = 0;
    harddisk_io_drive_head_register message_to_drive = {.value = 0};

    // Set port of drive
    if (bus == HARDDISK_ATA_PRIMARY_BUS)
    {
        io_port = HARDDISK_ATA_PRIMARY_BUS_IO_PORT;
        control_port = HARDDISK_ATA_PRIMARY_BUS_CONTROL_PORT;
    }
    else if(bus == HARDDISK_ATA_SECONDARY_BUS)
    {
        io_port = HARDDISK_ATA_SECONDARY_BUS_IO_PORT;
        control_port = HARDDISK_ATA_SECONDARY_BUS_CONTROL_PORT;
    }
    else 
    {
        return -2;
    }

    // Set drive
    switch (type)
    {
    case HARDDISK_ATA_MASTER:
        // For master set it to 0xA0. We set 2 bits that should be always 1.
        message_to_drive.fields.always_set_field_1 = 1;
        message_to_drive.fields.always_set_field_2 = 1;
        break;
    case HARDDISK_ATA_SLAVE:
        // For slave set it to 0xB0. We set 2 bits that should be always 1 and drive number to 1.
        message_to_drive.fields.always_set_field_1 = 1;
        message_to_drive.fields.always_set_field_2 = 1;
        message_to_drive.fields.drive_number = 1;
        break;
    default:
        return -2;
    }


    // Send message to drive
    io_out_byte(io_port + HARDDISK_IO_DRIVE_HEAD_REGISTER_OFFSET, message_to_drive.value);

    // Make 400ns delay
    __harddisk_400ns_delay(control_port);

    // Set the Sectorcount, LBAlo, LBAmid, and LBAhi IO ports to 0
    io_out_word(io_port + HARDDISK_IO_SECTOR_COUNT_REGISTER_OFFSET, 0);
    io_out_word(io_port + HARDDISK_IO_SECTOR_NUMBER_REGISTER_OFFSET, 0);
    io_out_word(io_port + HARDDISK_IO_CYLINDER_LOW_REGISTER_OFFSET, 0);
    io_out_word(io_port + HARDDISK_IO_CYLINDER_HIGH_REGISTER_OFFSET, 0);

    // Send the specific IDENTIFY command to the Command IO port.
    io_out_byte(io_port + HARDDISK_IO_COMMAND_REGISTER_OFFSET, identify_command);


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
                //Check LBA mid and high to check if device follows ATA specification
                uint8_t LBA_mid, LBA_hi;
                LBA_mid = io_in_byte(io_port + HARDDISK_IO_CYLINDER_LOW_REGISTER_OFFSET);
                LBA_hi = io_in_byte(io_port + HARDDISK_IO_CYLINDER_HIGH_REGISTER_OFFSET);
                if(LBA_hi != 0 && LBA_mid != 0) return 1;
                //TODO
                //There should be a way to load the data from devices that do not follow the standard. Find this way

                // Otherwise, continue polling one of the Status ports until bit 3 (DRQ, value = 8) sets, or until bit 0 (ERR, value = 1) sets.
                for(;;)
                {
                    result.value = io_in_byte(io_port + HARDDISK_IO_STATUS_REGISTER_OFFSET);


                    if(result.fields.has_pio_data_to_transfer_or_ready_to_accept_pio_data == 1 || result.fields.overlapped_mode_service_request == 1)
                    {
                        for(int i = 0; i < 256; i++)
                        {
                            //  Read 256 16-bit values, and store them.
                            data->values[i] = io_in_word(io_port);
                        }
                        return 1;
                    }
                    else if(result.fields.error_occurred == 1 || result.fields.drive_fault_error == 1)
                    {
                        return -1;
                    }
                }
                
            }   
            result.value = io_in_byte(io_port + HARDDISK_IO_STATUS_REGISTER_OFFSET);
        }
    }
}