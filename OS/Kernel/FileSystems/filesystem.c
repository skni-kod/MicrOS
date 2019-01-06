#include "filesystem.h"

bool filesystem_get_file_info(char *path, filesystem_file_info *file_info)
{
    return fat_generic_get_file_info(path, file_info);
}

bool filesystem_get_directory_info(char *path, filesystem_directory_info *directory_info)
{
    return fat_generic_get_directory_info(path, directory_info);
}

bool filesystem_read_file(char *path, uint8_t *buffer, uint32_t start_index, uint32_t length)
{
    return fat_generic_read_file(path, buffer, start_index, length);
}

uint32_t filesystem_get_entries_count_in_directory(char *path)
{
    return fat_generic_get_entries_count_in_directory(path);
}

bool filesystem_get_entries_in_directory(char *path, char *entries)
{
    return fat_generic_get_entries_in_directory(path, entries);
}