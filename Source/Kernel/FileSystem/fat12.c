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
    bool dot_detected = false;

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
    while(sector != 0xFF && sector != 0xFFF)
    {
        buffer = realloc(buffer, 512 * (*read_sectors_count + 1));

        uint8_t* read_data = floppy_read_sector(sector + 31);
        sector = fat12_read_sector_value(sector);

        memcpy(buffer + (*read_sectors_count * 512), read_data, 512);
        (*read_sectors_count)++;
    }

    return buffer;
}

directory_entry* fat12_get_directory_from_path(char* path)
{
    vector* chunks = fat12_parse_path(path);
    directory_entry* directory = fat12_get_directory_from_chunks(chunks);
    
    vector_clear(chunks);
    free(chunks);

    return directory;
}

directory_entry* fat12_get_directory_from_chunks(vector* chunks)
{
    directory_entry* current_directory = malloc(directory_length);
    memcpy(current_directory, root, directory_length);

    directory_entry* result = NULL;

    if(chunks->count == 0)
    {
        return current_directory;
    }

    directory_entry* current_file_ptr = current_directory;
    uint32_t current_chunk_index = 0;

    for(int i=0; i<fat_header_data->directory_entries; i++)
    {
        uint8_t full_filename[12];
        fat12_merge_filename_and_extension(current_file_ptr, full_filename);

        if(fat12_is_entry_valid(current_file_ptr) && current_file_ptr->file_attributes.subdirectory)
        {
            if(memcmp(full_filename, chunks->data[current_chunk_index], 12) == 0)
            {
                uint16_t read_sectors_count = 0;
                uint8_t* directory = fat12_load_file_from_sector(current_file_ptr->first_sector, &read_sectors_count);

                free(current_directory);

                current_directory = directory;
                current_file_ptr = current_directory;

                if(current_chunk_index == chunks->count - 1)
                {
                    result = current_directory;
                    break;
                }
                else
                {
                    i = 0;
                    current_chunk_index++;
                }
            }
        }

        if(i + 1 == fat_header_data->directory_entries)
        {
            result = NULL;
        }

        current_file_ptr++;
    }
    
    return result;
}

uint8_t* fat12_read_file(char* path, uint32_t read_sectors, uint32_t* read_size)
{
    directory_entry* file_info = fat12_get_info(path, false);
    uint8_t* result = NULL;

    if(file_info != NULL)
    {
        uint8_t* file_content = fat12_load_file_from_sector(file_info->first_sector, read_sectors);
        *read_size = file_info->size;

        result = file_content;
        free(file_info);
    }

    return result;
}

directory_entry* fat12_get_info(char* path, bool is_directory)
{
    vector* chunks = fat12_parse_path(path);
    char* target_filename = chunks->data[chunks->count - 1];

    vector_remove(chunks, chunks->count - 1);

    directory_entry* directory = fat12_get_directory_from_chunks(chunks);
    directory_entry* current_file_ptr = directory;
    directory_entry* result = NULL;

    for(int i=0; i<fat_header_data->directory_entries; i++)
    {
        uint8_t full_filename[12];
        fat12_merge_filename_and_extension(current_file_ptr, full_filename);

        if((fat12_is_entry_valid(current_file_ptr) && current_file_ptr->file_attributes.subdirectory == is_directory))
        {
            if(memcmp(full_filename, target_filename, 12) == 0)
            {
                result = current_file_ptr;
                break;
            }
        }

        if(i + 1 == fat_header_data->directory_entries)
        {
            result = NULL;
            break;
        }

        current_file_ptr++;
    }

    directory_entry* result_without_junk = NULL;
    if(result != NULL)
    {
        result_without_junk = malloc(sizeof(directory_entry));
        memcpy(result_without_junk, result, sizeof(directory_entry));
    }

    vector_clear(chunks);
    free(chunks);
    free(directory); 
    free(target_filename);

    return result_without_junk;
}

bool fat12_is_entry_valid(directory_entry* entry)
{
    return entry->filename[0] >= 32 && entry->filename[0] <= 126;
}

void fat12_merge_filename_and_extension(directory_entry* entry, uint8_t* buffer)
{
    memset(buffer, ' ', 12);
    memcpy(buffer, entry->filename, 8);

    if(!entry->file_attributes.subdirectory)
    {
        buffer[8] = '.';
        memcpy(buffer + 9, entry->extension, 3);
    }
}