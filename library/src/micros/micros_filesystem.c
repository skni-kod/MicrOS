#include "micros_filesystem.h"

bool micros_filesystem_get_file_info(const char *path, micros_filesystem_file_info *file_info)
{
    return micros_interrupt_2a(0x0500, (uint32_t)path, (uint32_t)file_info);
}

bool micros_filesystem_get_directory_info( const char *path, micros_filesystem_directory_info *directory_info)
{
    return micros_interrupt_2a(0x0501, (uint32_t)path, (uint32_t)directory_info);
}

bool micros_filesystem_read_file( const char *path, uint8_t *buffer,  const uint32_t start_index,  const uint32_t length)
{
    return micros_interrupt_4a(0x0502, (uint32_t)path, (uint32_t)buffer, start_index, length);
}

uint32_t micros_filesystem_get_entries_count_in_directory(const char *path)
{
    return micros_interrupt_1a(0x0503, (uint32_t)path);
}

bool micros_filesystem_get_entries_in_directory(const char *path, char **entries)
{
    return micros_interrupt_2a(0x0504, (uint32_t)path, (uint32_t)entries);
}

bool micros_filesystem_is_file(const char *path)
{
    return micros_interrupt_1a(0x0505, (uint32_t)path);
}

bool micros_filesystem_is_directory(const char *path)
{
    return micros_interrupt_1a(0x0506, (uint32_t)path);
}

bool micros_filesystem_create_file(const char *path)
{
    return micros_interrupt_1a(0x0507, (uint32_t)path);
}

bool micros_filesystem_create_directory(const char *path)
{
    return micros_interrupt_1a(0x0508, (uint32_t)path);
}

bool micros_filesystem_delete_file(const char *path)
{
    return micros_interrupt_1a(0x0509, (uint32_t)path);
}

bool micros_filesystem_delete_directory(const char *path)
{
    return micros_interrupt_1a(0x050A, (uint32_t)path);
}

bool micros_filesystem_rename_file(const char *path, const char *new_name)
{
    return micros_interrupt_2a(0x050B, (uint32_t)path, (uint32_t)new_name);
}

bool micros_filesystem_rename_directory(const char *path, const char *new_name)
{
    return micros_interrupt_2a(0x050C, (uint32_t)path, (uint32_t)new_name);
}

bool micros_filesystem_save_to_file(const char *path, const char *buffer, const int size)
{
    return micros_interrupt_3a(0x050D, (uint32_t)path, (uint32_t)buffer, size);
}

bool micros_filesystem_append_to_file(const char *path, const char *buffer, const int size)
{
    return micros_interrupt_3a(0x050E, (uint32_t)path, (uint32_t)buffer, size);
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
    return micros_interrupt_1a(0x050F, (uint32_t)partition_symbol);
}

int micros_filesystem_get_total_space(const char partition_symbol)
{
    return micros_interrupt_1a(0x0510, (uint32_t)partition_symbol);
}