#include "fat12.h"

volatile floppy_header* fat_header_data = FLOPPY_HEADER_DATA;
volatile char* fat;

uint32_t fat_length;

void fat12_init()
{
    fat_length = fat_header_data->bytes_per_sector * fat_header_data->sectors_per_fat;
    fat = malloc(fat_length);

    fat12_load_fat();
}

void fat12_load_fat()
{
    for(int i=0; i<fat_header_data->sectors_per_fat; i++)
    {
        uint8_t* buffer = floppy_read_sector(i + 1);
        memcpy(fat + (i * 512), buffer, 512);
    }
}