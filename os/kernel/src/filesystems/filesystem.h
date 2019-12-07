#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include <stdbool.h>
#include "fat/fat.h"
#include "generic/filesystem_file_info.h"
#include "generic/filesystem_directory_info.h"
#include "klibrary/kvector.h"
#include "drivers/floppy/floppy.h"
#include "drivers/harddisk/harddisk.h"
#include "partitions/partitions.h"

// This is the common entry point for all filesystems. Now it will support only FAT, so every
// method with fat_ prefix will be called directly. It will be changed in the future if there
// will be more supported file systems.
void filesystem_remove_partition_from_path(char *old_path, char *new_path);
bool filesystem_get_file_info(char *path, filesystem_file_info *file_info);
bool filesystem_get_directory_info(char *path, filesystem_directory_info *directory_info);
bool filesystem_read_file(char *path, uint8_t *buffer, uint32_t start_index, uint32_t length);
uint32_t filesystem_get_entries_count_in_directory(char *path);
bool filesystem_get_entries_in_directory(char *path, char **entries);
bool filesystem_is_file(char *path);
bool filesystem_is_directory(char *path);
uint32_t filesystem_get_free_space(char partition_symbol);
uint32_t filesystem_get_total_space(char partition_symbol);

bool filesystem_create_file(char *path);
bool filesystem_create_directory(char *path);
bool filesystem_delete_file(char *path);
bool filesystem_delete_directory(char *path);
bool filesystem_rename_file(char *path, char *new_name);
bool filesystem_rename_directory(char *path, char *new_name);
bool filesystem_save_to_file(char *path, char *buffer, int size);
bool filesystem_append_to_file(char *path, char *buffer, int size);

void filesystem_remove_partition_from_path(char *old_path, char *new_path);

#endif