#include "micros_filesystem.h"

bool micros_filesystem_get_file_info(char *path, micros_filesystem_file_info *file_info)
{
    return micros_interrupt_2a(0x50, (uint32_t)path, (uint32_t)file_info);
}

bool micros_filesystem_get_directory_info(char *path, micros_filesystem_directory_info *directory_info)
{
    return micros_interrupt_2a(0x51, (uint32_t)path, (uint32_t)directory_info);
}

bool micros_filesystem_read_file(char *path, uint8_t *buffer, uint32_t start_index, uint32_t length)
{
    return micros_interrupt_4a(0x52, (uint32_t)path, (uint32_t)buffer, start_index, length);
}

uint32_t micros_filesystem_get_entries_count_in_directory(char *path)
{
    return micros_interrupt_1a(0x53, (uint32_t)path);
}

bool micros_filesystem_get_entries_in_directory(char *path, char *entries)
{
    return micros_interrupt_2a(0x54, (uint32_t)path, (uint32_t)entries);
}

bool micros_filesystem_is_file(char *path)
{
    return micros_interrupt_1a(0x55, (uint32_t)path);
}

bool micros_filesystem_is_directory(char *path)
{
    return micros_interrupt_1a(0x56, (uint32_t)path);
}