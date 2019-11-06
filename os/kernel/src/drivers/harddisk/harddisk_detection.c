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
    __harddisk_400ns_delay(io_port);

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
        __harddisk_atapi_check_presence(type, bus, data);
        return HARDDISK_ATAPI_PRESENT;
    }
	if (cylinder_low==0 && cylinder_high == 0)
    {
        __harddisk_ata_check_presence(type, bus, data);
        return HARDDISK_ATA_PRESENT;
    }

    /*// Set the Sectorcount, LBAlo, LBAmid, and LBAhi IO ports to 0
    io_out_word(io_port + HARDDISK_IO_SECTOR_COUNT_REGISTER_OFFSET, 0);
    io_out_word(io_port + HARDDISK_IO_SECTOR_NUMBER_REGISTER_OFFSET, 0);
    io_out_word(io_port + HARDDISK_IO_CYLINDER_LOW_REGISTER_OFFSET, 0);
    io_out_word(io_port + HARDDISK_IO_CYLINDER_HIGH_REGISTER_OFFSET, 0);

    // Send the IDENTIFY command (0xEC) to the Command IO port.
    io_out_byte(io_port + HARDDISK_IO_COMMAND_REGISTER_OFFSET, HARDDISK_IDENTIFY_DEVICE_COMMAND);

    // Read the Status port again.
    harddisk_io_control_status_register result;
    result.value = io_in_byte(io_port + HARDDISK_IO_STATUS_REGISTER_OFFSET);

    // If the value read is 0, the drive does not exist.
    if(result.value == 0)
    {
        return HARDDISK_NOT_PRESENT;
    }  */
	
	return HARDDISK_NOT_PRESENT;
}