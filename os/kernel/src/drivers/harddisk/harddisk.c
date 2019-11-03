#include "harddisk.h"


bool check_harddisk_presence(uint8_t master, uint8_t bus)
{
    uint16_t io_port = 0;
    uint16_t message_to_drive = 0;

    // Set port of drive
    if (bus == 1)
    {
        io_port = harddisk_primary_bus_io_port;
    }
    else if(bus == 2)
    {
        io_port = harddisk_secondary_bus_io_port;
    }
    else
    {
        return;
    }

    // Set drive
    switch (master)
    {
    case 0:
        message_to_drive = 0xB0;
        break;
    case 1:
        message_to_drive = 0xA0;
        break;
    default:
        return false;
    }

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
    uint8_t value = io_in_byte(0x1F7);

    // If the value read is 0, the drive does not exist.
    if(value == 0)
    {
        return false;
    }
    // For any other value: poll the Status port (0x1F7) until bit 7 (BSY, value = 0x80) clears.
    else
    {
        return true;
    }
}