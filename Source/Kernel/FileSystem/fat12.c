#include "fat12.h"

volatile floppy_header* fat_header_data = FLOPPY_HEADER_DATA;
volatile uint8_t* fat;
volatile uint8_t* root;

uint32_t fat_length;
uint32_t directory_length;

void fat12_init()
{
    fat_length = fat_header_data->bytes_per_sector * fat_header_data->sectors_per_fat;
    fat = malloc(fat_length);

    directory_length = fat_header_data->directory_entries * 32;
    root = malloc(directory_length);

    fat12_load_fat();
    fat12_load_root();
}

void fat12_load_fat()
{
    for(int i=1; i<fat_header_data->sectors_per_fat; i++)
    {
        uint8_t* buffer = floppy_read_sector(i);
        memcpy((uint32_t)fat + ((i - 1) * 512), buffer, 512);
    }
}

void fat12_load_root()
{
    uint8_t root_first_sector = 1 + (fat_header_data->sectors_per_fat * 2);
    uint8_t root_sectors_count = (fat_header_data->directory_entries * 32) / fat_header_data->bytes_per_sector;

    for(int i=root_first_sector; i<root_first_sector + root_sectors_count; i++)
    {
        uint8_t* buffer = floppy_read_sector(i);
        memcpy((uint32_t)root + ((i - root_first_sector) * 512), buffer, 512);
    }

    for(int i=0; i<512; i++)
    {
        vga_printchar(root[i]);
    }
}

uint16_t fat12_read_sector_value(uint32_t sector_number)
{
    uint8_t high_byte;
    uint8_t low_byte;

    if(sector_number % 2 == 0)
    {
        high_byte = *(fat + (uint32_t)(sector_number * 1.5f + 1 )) & 0x0F;
        low_byte = *(fat + (uint32_t)(sector_number * 1.5f));

        return high_byte << 4 | low_byte;
    }
    else
    {
        high_byte = *(fat + (uint32_t)(((sector_number - 1) * 1.5f) + 2));
        low_byte = *(fat + (uint32_t)((sector_number - 1) * 1.5f) + 1) & 0xF0;

        return high_byte << 4 | low_byte >> 4;
    }
}