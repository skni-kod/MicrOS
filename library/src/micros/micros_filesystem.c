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

bool micros_filesystem_get_entries_in_directory(char *path, char **entries)
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

bool micros_filesystem_create_file(char *path)
{
    return micros_interrupt_1a(0x57, (uint32_t)path);
}

bool micros_filesystem_create_directory(char *path)
{
    return micros_interrupt_1a(0x58, (uint32_t)path);
}

bool micros_filesystem_delete_file(char *path)
{
    return micros_interrupt_1a(0x59, (uint32_t)path);
}

bool micros_filesystem_delete_directory(char *path)
{
    return micros_interrupt_1a(0x5A, (uint32_t)path);
}

bool micros_filesystem_rename_file(char *path, char *new_name)
{
    return micros_interrupt_2a(0x5B, (uint32_t)path, (uint32_t)new_name);
}

bool micros_filesystem_rename_directory(char *path, char *new_name)
{
    return micros_interrupt_2a(0x5C, (uint32_t)path, (uint32_t)new_name);
}

bool micros_filesystem_save_to_file(char *path, char *buffer, int size)
{
    return micros_interrupt_3a(0x5D, (uint32_t)path, (uint32_t)buffer, size);
}

bool micros_filesystem_append_to_file(char *path, char *buffer, int size)
{
    return micros_interrupt_3a(0x5E, (uint32_t)path, (uint32_t)buffer, size);
}

bool micros_filesystem_file_exists(char *path)
{
    micros_filesystem_file_info info;
    return micros_filesystem_get_file_info(path, &info);
}

bool micros_filesystem_directory_exists(char *path)
{
    micros_filesystem_directory_info info;
    return micros_filesystem_get_directory_info(path, &info);
}

int micros_filesystem_get_free_space()
{
    return micros_interrupt_0a(0x70);
}

int micros_filesystem_get_total_space()
{
    return micros_interrupt_0a(0x71);
}