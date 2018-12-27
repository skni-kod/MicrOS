#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include <stdbool.h>
#include "Generic/filesystem_file_info.h"
#include "Generic/filesystem_directory_info.h"

// This is the common entry point for all filesystems. Now it will support only FAT, so every
// method with fat_ prefix will be called directly. It will be changed in the future if there
// will be more supported file systems.

bool filesystem_get_file_info(char *path, filesystem_file_info *file_info);
bool filesystem_get_directory_info(char *path, filesystem_directory_info *directory_info);

#endif