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

bool filesystem_get_entries_in_directory(char *path, char **entries)
{
    return fat_generic_get_entries_in_directory(path, entries);
}

bool filesystem_is_file(char *path)
{
    return fat_generic_is_file(path);
}

bool filesystem_is_directory(char *path)
{
    return fat_generic_is_directory(path);
}

uint32_t filesystem_get_free_space()
{
    return fat_generic_get_free_space();
}

uint32_t filesystem_get_total_space()
{
    return fat_generic_get_total_space();
}

bool filesystem_create_file(char *path)
{
    return fat_generic_create_file(path);
}

bool filesystem_create_directory(char *path)
{
    return fat_generic_create_directory(path);
}

bool filesystem_delete_file(char *path)
{
    return fat_generic_delete_file(path);
}

bool filesystem_delete_directory(char *path)
{
    return fat_generic_delete_directory(path);
}

bool filesystem_rename_file(char *path, char *new_name)
{
    return fat_generic_rename_file(path, new_name);
}

bool filesystem_rename_directory(char *path, char *new_name)
{
    return fat_generic_rename_directory(path, new_name);
}

bool filesystem_save_to_file(char *path, char *buffer, int size)
{
    return fat_generic_save_to_file(path, buffer, size);
}

bool filesystem_append_to_file(char *path, char *buffer, int size)
{
    return fat_generic_append_to_file(path, buffer, size);
}