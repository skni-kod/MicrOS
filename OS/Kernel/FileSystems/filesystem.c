#include "filesystem.h"

void fs_get_file_info(char *path, fs_file_info *file_info)
{
    fat_generic_get_file_info(path, file_info);
}

void fs_get_directory_info(char *path, fs_directory_info *directory_info)
{
    fat_generic_get_directory_info(path, directory_info);
}