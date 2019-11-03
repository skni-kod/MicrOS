#include "harddisk.h"


uint8_t check_harddisk_presence(uint8_t master, uint8_t bus)
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
        return -2;
    }

    // Set drive
    switch (master)
    {
    case 1:
        message_to_drive = 0xA0;
        break;
    case 2:
        message_to_drive = 0xB0;
        break;
    default:
        return -2;
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
        return 0;
    }  
    else
    {   // For any other value: poll the Status port (0x1F7) until bit 7 (BSY, value = 0x80) clears.
        for(;;)
        {
            harddisk_io_control_status_register *status = (harddisk_io_control_status_register *) &value;
            if(status->busy == 0)
            {
                // Otherwise, continue polling one of the Status ports until bit 3 (DRQ, value = 8) sets, or until bit 0 (ERR, value = 1) sets.
                for(;;)
                {
                    value = io_in_byte(0x1F7);
                    status = (harddisk_io_control_status_register *) &value;
                    if(status->has_pio_data_to_transfer_or_ready_to_accept_pio_data == 1)
                    {
                        return 1;
                    }
                    else if(status->error_occurred)
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
            value = io_in_byte(0x1F7);
        }
    }
}