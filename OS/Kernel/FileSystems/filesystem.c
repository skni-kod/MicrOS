#include "filesystem.h"

bool filesystem_get_file_info(char *path, filesystem_file_info *file_info)
{
    return fat_generic_get_file_info(path, file_info);
}

bool filesystem_get_directory_info(char *path, filesystem_directory_info *directory_info)
{
    return fat_generic_get_directory_info(path, directory_info);
}