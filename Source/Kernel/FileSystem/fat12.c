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
    vector_init(chunks);

    uint8_t index = 0;

    while(*path != 0)
    {
        if(*path == '/')
        {
            char* string = calloc(12, 1);
            memset(string, ' ', 12);
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

    for(int i=0; i<chunks->count; i++)
    {
        fat12_normalise_filename(chunks->data[i]);
    }

    if(index == 0)
    {
        free(chunks->data[chunks->count - 1]);
        vector_remove(chunks, chunks->count - 1);
    }

    return chunks;
}

void fat12_normalise_filename(char* filename)
{
    char* ptr = filename;
    for(int i=0; i<12; i++)
    {
        if(*ptr == '.')
        {
            if(i < 9)
            {
                memmove(filename + 8, ptr, 4);
                memset(filename + i, ' ', 11 - i - 3);
                break;
            }
        }

        ptr++;
    }
}

uint8_t* fat12_load_file_from_sector(uint16_t sector, uint16_t* read_sectors_count)
{
    uint8_t* buffer = malloc(512);

    *read_sectors_count = 0;
    while(sector != 0xFFF)
    {
        buffer = realloc(buffer, 512 * (*read_sectors_count + 1));

        uint8_t* read_data = floppy_read_sector(sector + 31);
        sector = fat12_read_sector_value(sector);

        memcpy(buffer + (*read_sectors_count * 512), read_data, 512);
        (*read_sectors_count)++;
    }

    return buffer;
}

vector* fat12_list(char* path)
{
    vector* chunks = fat12_parse_path(path);
    directory_entry* current_file = root;
    uint32_t current_chunk_index = 0;

    for(int i=0; i<fat_header_data->directory_entries; i++)
    {
        uint8_t full_filename[12];
        memset(full_filename, ' ', 12);

        //full_filename[8] = '.';

        memcpy(full_filename, current_file->filename, 8);
        memcpy(full_filename + 9, current_file->extension, 3);

        uint8_t first_filename_char = current_file->filename[0];
        if(first_filename_char != 0 && first_filename_char != 229)
        {
            if(memcmp(full_filename, chunks->data[current_chunk_index], 12) == 0)
            {
                uint16_t read_sectors_count = 0;
                uint8_t* directory = fat12_load_file_from_sector(current_file->first_sector, &read_sectors_count);

                current_file = directory;
                if(current_chunk_index == chunks->count - 1)
                {
                    break;
                }
                else
                {
                    i = 0;
                }
            }
        }

        current_file++;
    }

    vector* files = malloc(sizeof(vector));
    vector_init(files);

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