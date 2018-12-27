#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include <stdbool.h>
#include "FAT/fat.h"

// This is the common entry point for all filesystems. Now it will support only FAT, so every
// method with fat_ prefix will be called directly. It will be changed in the future if there
// will be more supported file systems.

void fs_get_file_info(char *path, fs_file_info *file_info);
void fs_get_directory_info(char *path, fs_directory_info *directory_info);

#endif