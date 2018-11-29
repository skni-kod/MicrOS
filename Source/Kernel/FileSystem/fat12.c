#include "fat12.h"

volatile floppy_header* fat_header_data = FLOPPY_HEADER_DATA;
volatile uint8_t* fat;
volatile directory_entry* root;

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

vector* fat12_parse_path(char* path)
{
    vector* chunks = malloc(sizeof(vector));
    uint8_t index = 0;

    while(*path != 0)
    {
        if(*path == '/')
        {
            char* string = calloc(12, 1);
            vector_add(chunks, string);

            index = 0;
        }
        else
        {
            char* string = chunks->data[chunks->count - 1];
            string[index] = *path;
            index++;
        }

        path++;
    }

    if(index == 0)
    {
        free(chunks->data[chunks->count - 1]);
        vector_remove(chunks, chunks->count - 1);
    }

    return chunks;
}

vector* fat12_list(char* path)
{
    vector* chunks = fat12_parse_path(path);
    directory_entry* current_file = root;
    uint32_t current_chunk_index = 0;

    /*for(int i=0; i<fat_header_data->directory_entries; i++)
    {
        uint8_t full_filename[12];
        full_filename[9] = '.';

        memcpy(full_filename, current_file->filename, 8);
        memcpy(full_filename + 9, current_file->extension, 3);

        current_file++;
    }*/

    vector* files = malloc(sizeof(vector));
    for(int i=0; i<fat_header_data->directory_entries; i++)
    {
        uint8_t first_filename_char = current_file->filename[0];
        if(first_filename_char != 0 && first_filename_char != 229)
        {
            vector_add(files, current_file);
        }
        current_file++;
    }

    vector_clear(chunks);
    free(chunks);
    
    return files;
}