#include "fat.h"

// TODO: Add functions related with saving/renaming/other important stuff.

volatile floppy_header *fat_header_data = (floppy_header *)FLOPPY_HEADER_DATA;
volatile uint8_t *fat;
volatile fat_directory_entry *root;

uint32_t fat_length;
uint32_t directory_length;

void fat_init()
{
    fat_length = fat_header_data->bytes_per_sector * fat_header_data->sectors_per_fat;
    fat = heap_kernel_alloc(fat_length, 0);

    directory_length = fat_header_data->directory_entries * 32;
    root = heap_kernel_alloc(directory_length, 0);

    fat_load_fat();
    fat_load_root();
}

void fat_load_fat()
{
    for (int i = 1; i < fat_header_data->sectors_per_fat; i++)
    {
        uint8_t *buffer = floppy_read_sector(i);
        memcpy((void *)((uint32_t)fat + ((i - 1) * 512)), buffer, 512);
    }
}

void fat_load_root()
{
    uint8_t root_first_sector = 1 + (fat_header_data->sectors_per_fat * 2);
    uint8_t root_sectors_count = (fat_header_data->directory_entries * 32) / fat_header_data->bytes_per_sector;

    for (int i = root_first_sector; i < root_first_sector + root_sectors_count; i++)
    {
        uint8_t *buffer = floppy_read_sector(i);
        memcpy((void *)((uint32_t)root + ((i - root_first_sector) * 512)), buffer, 512);
    }
}

uint16_t fat_read_sector_value(uint32_t sector_number)
{
    uint8_t high_byte;
    uint8_t low_byte;

    if (sector_number % 2 == 0)
    {
        high_byte = *(fat + (uint32_t)(sector_number * 1.5f + 1)) & 0x0F;
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

kvector *fat_parse_path(char *path)
{
    kvector *chunks = heap_kernel_alloc(sizeof(kvector), 0);
    kvector_init(chunks);

    uint8_t index = 0;

    while (*path != 0)
    {
        if (*path == '/')
        {
            char *string = heap_kernel_alloc(12, 0);
            memset(string, ' ', 12);
            kvector_add(chunks, string);

            index = 0;
        }
        else
        {
            char *string = chunks->data[chunks->count - 1];
            string[index] = *path;
            index++;
        }

        path++;
    }

    for (uint32_t i = 0; i < chunks->count; i++)
    {
        fat_normalise_filename(chunks->data[i]);
    }

    if (index == 0)
    {
        heap_kernel_dealloc(chunks->data[chunks->count - 1]);
        kvector_remove(chunks, chunks->count - 1);
    }

    return chunks;
}

void fat_normalise_filename(char *filename)
{
    char *ptr = filename;
    for (int i = 0; i < 12; i++)
    {
        if (*ptr == '.')
        {
            if (i < 9)
            {
                memmove(filename + 8, ptr, 4);
                memset(filename + i, ' ', 11 - i - 3);
                break;
            }
        }

        ptr++;
    }
}

void fat_denormalise_filename(char *filename)
{
    char *ptr = filename;
    int spaces_count = 0;

    for (int i = 0; i < 12; i++)
    {
        if (*ptr == ' ')
        {
            spaces_count++;
        }
        else if (*ptr == '.')
        {
            memmove(ptr - spaces_count, ptr, 4);
            if (spaces_count > 0)
            {
                *(ptr + 4 - spaces_count) = 0;
            }

            return;
        }
        else if (*ptr == '/')
        {
            *(ptr - spaces_count) = '/';
            *(ptr - spaces_count + 1) = 0;
            return;
        }

        ptr++;
    }

    if (spaces_count != 0)
    {
        filename[12 - spaces_count] = 0;
    }
}

uint8_t *fat_load_file_from_sector(uint16_t initial_sector, uint16_t sector_offset, uint16_t sectors_count)
{
    uint16_t sector = initial_sector;
    for (int i = 0; i < sector_offset; i++)
    {
        sector = fat_read_sector_value(sector);
        if (sector == 0xFF || sector == 0xFFF)
        {
            return NULL;
        }
    }

    uint8_t *buffer = heap_kernel_alloc(512, 0);
    uint32_t read_sectors = 0;

    while (read_sectors < sectors_count && sector < 0xFF0)
    {
        buffer = heap_kernel_realloc(buffer, 512 * (read_sectors + 1), 0);

        uint8_t *read_data = floppy_read_sector(sector + 31);
        sector = fat_read_sector_value(sector);

        memcpy(buffer + (read_sectors * 512), read_data, 512);
        read_sectors++;
    }

    return buffer;
}

bool fat_read_file(char *path, uint8_t *buffer, uint32_t start_index, uint32_t length)
{
    fat_directory_entry *file_info = fat_get_info(path, false);

    if (file_info == NULL)
    {
        return false;
    }

    uint16_t initial_sector = start_index / 512;
    uint16_t last_sector = (start_index + length) / 512;
    uint16_t sectors_count = length == 0 ? file_info->size / 512 + 1 : (uint16_t)(last_sector - initial_sector + 1);

    uint8_t *result = fat_load_file_from_sector(file_info->first_sector, initial_sector, sectors_count);
    memcpy(buffer, result + (start_index % 512), length);

    heap_kernel_dealloc(result);
    heap_kernel_dealloc(file_info);

    return true;
}

fat_directory_entry *fat_get_directory_from_path(char *path)
{
    kvector *chunks = fat_parse_path(path);
    fat_directory_entry *directory = fat_get_directory_from_chunks(chunks);

    kvector_clear(chunks);
    heap_kernel_dealloc(chunks);

    return directory;
}

fat_directory_entry *fat_get_directory_from_chunks(kvector *chunks)
{
    fat_directory_entry *current_directory = heap_kernel_alloc(directory_length, 0);
    memcpy(current_directory, (void *)root, directory_length);

    fat_directory_entry *result = NULL;

    if (chunks->count == 0)
    {
        return current_directory;
    }

    fat_directory_entry *current_file_ptr = current_directory;
    uint32_t current_chunk_index = 0;

    for (int i = 0; i < fat_header_data->directory_entries; i++)
    {
        char full_filename[12];
        fat_merge_filename_and_extension(current_file_ptr, full_filename);

        if (fat_is_entry_valid(current_file_ptr) && current_file_ptr->file_attributes.subdirectory)
        {
            if (memcmp(full_filename, chunks->data[current_chunk_index], 12) == 0)
            {
                uint32_t sectors_to_load = fat_header_data->directory_entries * 32 / 512;
                uint8_t *directory = fat_load_file_from_sector(current_file_ptr->first_sector, 0, sectors_to_load);

                heap_kernel_dealloc(current_directory);

                current_directory = (fat_directory_entry *)directory;
                current_file_ptr = current_directory;

                if (current_chunk_index == chunks->count - 1)
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

        if (i + 1 == fat_header_data->directory_entries)
        {
            result = NULL;
        }

        current_file_ptr++;
    }

    return result;
}

fat_directory_entry *fat_get_info(char *path, bool is_directory)
{
    kvector *chunks = fat_parse_path(path);
    char *target_filename = chunks->data[chunks->count - 1];

    kvector_remove(chunks, chunks->count - 1);

    fat_directory_entry *directory = fat_get_directory_from_chunks(chunks);
    fat_directory_entry *current_file_ptr = directory;
    fat_directory_entry *result = NULL;

    for (int i = 0; i < fat_header_data->directory_entries; i++)
    {
        char full_filename[12];
        fat_merge_filename_and_extension(current_file_ptr, full_filename);

        if ((fat_is_entry_valid(current_file_ptr) && current_file_ptr->file_attributes.subdirectory == is_directory))
        {
            if (memcmp(full_filename, target_filename, 12) == 0)
            {
                result = current_file_ptr;
                break;
            }
        }

        if (i + 1 == fat_header_data->directory_entries)
        {
            result = NULL;
            break;
        }

        current_file_ptr++;
    }

    fat_directory_entry *result_without_junk = NULL;
    if (result != NULL)
    {
        result_without_junk = heap_kernel_alloc(sizeof(fat_directory_entry), 0);
        memcpy(result_without_junk, result, sizeof(fat_directory_entry));
    }

    kvector_clear(chunks);
    heap_kernel_dealloc(chunks);
    heap_kernel_dealloc(directory);
    heap_kernel_dealloc(target_filename);

    return result_without_junk;
}

uint32_t fat_get_entries_count_in_directory(char *path)
{
    kvector *chunks = fat_parse_path(path);

    fat_directory_entry *directory = fat_get_directory_from_chunks(chunks);
    fat_directory_entry *current_file_ptr = directory;
    uint32_t entries_count = 0;

    if (directory == NULL)
    {
        return 0;
    }

    for (int i = 0; i < fat_header_data->directory_entries; i++)
    {
        if (fat_is_entry_valid(current_file_ptr))
        {
            entries_count++;
        }

        if (i + 1 == fat_header_data->directory_entries)
        {
            break;
        }

        current_file_ptr++;
    }

    kvector_clear(chunks);
    heap_kernel_dealloc(chunks);
    heap_kernel_dealloc(directory);

    return entries_count;
}

uint32_t fat_get_entries_in_directory(char *path, char **entries)
{
    kvector *chunks = fat_parse_path(path);
    uint32_t path_length = strlen(path);

    fat_directory_entry *directory = fat_get_directory_from_chunks(chunks);
    fat_directory_entry *current_file_ptr = directory;
    uint32_t current_entry_index = 0;

    if (directory == NULL)
    {
        return false;
    }

    for (int i = 0; i < fat_header_data->directory_entries; i++)
    {
        if (fat_is_entry_valid(current_file_ptr))
        {
            char full_filename[12];
            fat_merge_filename_and_extension(current_file_ptr, full_filename);

            if (current_file_ptr->file_attributes.subdirectory)
            {
                full_filename[11] = '/';
            }

            fat_denormalise_filename(full_filename);

            entries[current_entry_index] = heap_kernel_alloc(path_length + 13, 0);
            memset(entries[current_entry_index], 0, path_length + 13);
            memcpy(entries[current_entry_index], path, path_length);
            memcpy(entries[current_entry_index] + path_length, full_filename, 12);

            current_entry_index++;
        }

        if (i + 1 == fat_header_data->directory_entries)
        {
            break;
        }

        current_file_ptr++;
    }

    kvector_clear(chunks);
    heap_kernel_dealloc(chunks);
    heap_kernel_dealloc(directory);

    return true;
}

bool fat_is_entry_valid(fat_directory_entry *entry)
{
    return entry->filename[0] >= 32 && entry->filename[0] <= 126 && entry->filename[0] != 0x2E;
}

void fat_merge_filename_and_extension(fat_directory_entry *entry, char *buffer)
{
    memset(buffer, ' ', 12);
    memcpy(buffer, entry->filename, 8);

    if (!entry->file_attributes.subdirectory)
    {
        buffer[8] = '.';
        memcpy(buffer + 9, entry->extension, 3);
    }
}

// Generic filesystem functions
bool fat_generic_get_file_info(char *path, filesystem_file_info *generic_file_info)
{
    fat_directory_entry *fat_file_info = fat_get_info(path, false);
    if (fat_file_info == NULL)
    {
        return false;
    }

    memcpy(generic_file_info->path, path, strlen(path) + 1);
    generic_file_info->size = fat_file_info->size;

    uint8_t filename_length = fat_generic_copy_filename_to_generic(fat_file_info->filename, generic_file_info->name);
    generic_file_info->name[filename_length] = '.';
    memcpy(generic_file_info->name + filename_length + 1, fat_file_info->extension, 3);

    fat_generic_convert_date_fat_to_generic(&fat_file_info->create_date, &fat_file_info->create_time, &generic_file_info->create_time);
    fat_generic_convert_date_fat_to_generic(&fat_file_info->modify_date, &fat_file_info->modify_time, &generic_file_info->modify_time);
    fat_generic_convert_date_fat_to_generic(&fat_file_info->last_access_date, NULL, &generic_file_info->access_time);

    heap_kernel_dealloc(fat_file_info);
    return true;
}

bool fat_generic_get_directory_info(char *path, filesystem_directory_info *generic_directory_info)
{
    fat_directory_entry *fat_directory_info = fat_get_info(path, true);
    if (fat_directory_info == NULL)
    {
        return false;
    }

    fat_generic_copy_filename_to_generic(fat_directory_info->filename, generic_directory_info->name);
    memcpy(generic_directory_info->path, path, strlen(path) + 1);

    fat_generic_convert_date_fat_to_generic(&fat_directory_info->create_date, &fat_directory_info->create_time, &generic_directory_info->create_time);

    heap_kernel_dealloc(fat_directory_info);
    return true;
}

bool fat_generic_read_file(char *path, uint8_t *buffer, uint32_t start_index, uint32_t length)
{
    return fat_read_file(path, buffer, start_index, length);
}

uint32_t fat_generic_get_entries_count_in_directory(char *path)
{
    return fat_get_entries_count_in_directory(path);
}

bool fat_generic_get_entries_in_directory(char *path, char **entries)
{
    return fat_get_entries_in_directory(path, entries);
}

bool fat_generic_is_file(char *path)
{
    fat_directory_entry *entry = fat_get_info(path, false);

    if (entry != NULL)
    {
        heap_kernel_dealloc(entry);
        return true;
    }

    return false;
}

bool fat_generic_is_directory(char *path)
{
    fat_directory_entry *entry = fat_get_info(path, true);

    if (entry != NULL)
    {
        heap_kernel_dealloc(entry);
        return true;
    }

    return false;
}

uint8_t fat_generic_copy_filename_to_generic(char *fat_filename, char *generic_filename)
{
    uint8_t filename_length = 0;
    for (filename_length = 0; filename_length < 8; filename_length++)
    {
        char c = fat_filename[filename_length];
        if (c == ' ')
        {
            break;
        }

        generic_filename[filename_length] = c;
    }

    return filename_length;
}

void fat_generic_convert_date_fat_to_generic(fat_directory_entry_date *fat_date, fat_directory_entry_time *fat_time, filesystem_time *generic_time)
{
    if (fat_date != NULL)
    {
        generic_time->year = fat_date->year + 1980;
        generic_time->month = fat_date->month;
        generic_time->day = fat_date->day;
    }

    if (fat_time != NULL)
    {
        generic_time->hour = fat_time->hours;
        generic_time->minutes = fat_time->minutes;
        generic_time->seconds = fat_time->seconds * 2;
    }
}