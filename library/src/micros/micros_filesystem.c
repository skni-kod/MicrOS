#include "micros_filesystem.h"

bool micros_filesystem_get_file_info(const char *path, micros_filesystem_file_info *file_info)
{
    return micros_interrupt_2a(0x50, (uint32_t)path, (uint32_t)file_info);
}

bool micros_filesystem_get_directory_info( const char *path, micros_filesystem_directory_info *directory_info)
{
    return micros_interrupt_2a(0x51, (uint32_t)path, (uint32_t)directory_info);
}

bool micros_filesystem_read_file( const char *path, uint8_t *buffer,  const uint32_t start_index,  const uint32_t length)
{
    return micros_interrupt_4a(0x52, (uint32_t)path, (uint32_t)buffer, start_index, length);
}

uint32_t micros_filesystem_get_entries_count_in_directory(const char *path)
{
    return micros_interrupt_1a(0x53, (uint32_t)path);
}

bool micros_filesystem_get_entries_in_directory(const char *path, char **entries)
{
    return micros_interrupt_2a(0x54, (uint32_t)path, (uint32_t)entries);
}

bool micros_filesystem_is_file(const char *path)
{
    return micros_interrupt_1a(0x55, (uint32_t)path);
}

bool micros_filesystem_is_directory(const char *path)
{
    return micros_interrupt_1a(0x56, (uint32_t)path);
}

bool micros_filesystem_create_file(const char *path)
{
    return micros_interrupt_1a(0x57, (uint32_t)path);
}

bool micros_filesystem_create_directory(const char *path)
{
    return micros_interrupt_1a(0x58, (uint32_t)path);
}

bool micros_filesystem_delete_file(const char *path)
{
    return micros_interrupt_1a(0x59, (uint32_t)path);
}

bool micros_filesystem_delete_directory(const char *path)
{
    return micros_interrupt_1a(0x5A, (uint32_t)path);
}

bool micros_filesystem_rename_file(const char *path, const char *new_name)
{
    return micros_interrupt_2a(0x5B, (uint32_t)path, (uint32_t)new_name);
}

bool micros_filesystem_rename_directory(const char *path, const char *new_name)
{
    return micros_interrupt_2a(0x5C, (uint32_t)path, (uint32_t)new_name);
}

bool micros_filesystem_save_to_file(const char *path, const char *buffer, const int size)
{
    return micros_interrupt_3a(0x5D, (uint32_t)path, (uint32_t)buffer, size);
}

bool micros_filesystem_append_to_file(const char *path, const char *buffer, const int size)
{
    return micros_interrupt_3a(0x5E, (uint32_t)path, (uint32_t)buffer, size);
}

bool micros_filesystem_file_exists(const char *path)
{
    micros_filesystem_file_info info;
    return micros_filesystem_get_file_info(path, &info);
}

bool micros_filesystem_directory_exists(const char *path)
{
    micros_filesystem_directory_info info;
    return micros_filesystem_get_directory_info(path, &info);
}

int micros_filesystem_get_free_space(const char partition_symbol)
{
    return micros_interrupt_1a(0x70, (uint32_t)partition_symbol);
}

int micros_filesystem_get_total_space(const char partition_symbol)
{
    return micros_interrupt_1a(0x71, (uint32_t)partition_symbol);
}