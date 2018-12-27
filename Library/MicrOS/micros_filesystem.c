#include "micros_filesystem.h"

bool micros_filesystem_get_file_info(char *path, micros_filesystem_file_info *file_info)
{
    return micros_interrupt_2a(0x50, (uint32_t)path, (uint32_t)file_info);
}

bool micros_filesystem_get_directory_info(char *path, micros_filesystem_directory_info *directory_info)
{
    return micros_interrupt_2a(0x51, (uint32_t)path, (uint32_t)directory_info);
}