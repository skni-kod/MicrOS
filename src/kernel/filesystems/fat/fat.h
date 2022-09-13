#ifndef FAT_H
#define FAT_H

#define FLOPPY_HEADER_DATA 0xc0007c00

#include <stdlib.h>
#include <kvector.h>
#include "fat_directory_entry.h"
#include "drivers/rtc/rtc.h"
#include "drivers/floppy/floppy.h"
#include "filesystems/fat/fat_header.h"
#include "filesystems/generic/filesystem_file_info.h"
#include "filesystems/generic/filesystem_directory_info.h"
#include "filesystems/partitions/partition.h"

bool fat_init();
void fat_load_fat();
void fat_save_fat();
void fat_load_root();
void fat_save_root();
uint16_t fat_read_cluster_value(uint32_t cluster_number);
void fat_save_cluster_value(uint32_t cluster_number, uint16_t value);
void fat_normalise_filename(char *filename, bool with_dot);
void fat_denormalise_filename(char *filename);

uint16_t fat_save_file_to_cluster(uint16_t initial_cluster, uint16_t clusters_count, char* buffer);

bool fat_read_file_from_path(char *path, uint8_t *buffer, uint32_t start_index, uint32_t length);
uint8_t *fat_read_file_from_cluster(uint16_t initial_cluster, uint16_t cluster_offset, uint16_t clusters_count, uint32_t *read_clusters);

bool fat_delete_file_from_path(char* path, bool is_directory);
bool fat_rename_file_from_path(char* path, char* new_name, bool is_directory);
bool fat_save_file_from_path(char* path, char* buffer, uint32_t size);
bool fat_append_file_from_path(char* path, char* buffer, uint32_t size);
bool fat_create_file_from_path(char* path, bool directory);

fat_directory_entry *fat_get_directory_from_path(char *path, uint32_t *read_clusters);
fat_directory_entry *fat_get_directory_from_chunks(kvector *chunks, uint32_t *read_clusters, bool *root_dir);
fat_directory_entry *fat_get_info_from_path(char *path, bool is_directory);
fat_directory_entry *fat_get_info_from_chunks(kvector *chunks, bool is_directory);
void fat_clear_file_clusters(uint32_t initial_cluster);
uint32_t fat_get_last_file_cluster(uint32_t initial_cluster);

uint32_t fat_get_entries_count_in_directory(char *path);
uint32_t fat_get_entries_in_directory(char *path, char **entries);

void fat_update_date(fat_directory_entry_date *fat_date, int year, int month, int day);
void fat_update_time(fat_directory_entry_time *fat_time, int hours, int minutes, int seconds);

kvector *fat_parse_path(char *path);
bool fat_is_entry_valid(fat_directory_entry *entry);
void fat_merge_filename_and_extension(fat_directory_entry *entry, char *buffer);
uint32_t fat_get_free_cluster_index();

// Generic filesystem functions
void fat_generic_set_current_partition(partition *partition);
bool fat_generic_get_file_info(char *path, filesystem_file_info *generic_file_info);
bool fat_generic_get_file_info(char *path, filesystem_file_info *generic_file_info);
bool fat_generic_get_directory_info(char *path, filesystem_directory_info *generic_directory_info);
bool fat_generic_read_file(char *path, uint8_t *buffer, uint32_t start_index, uint32_t length);
uint32_t fat_generic_get_entries_count_in_directory(char *path);
bool fat_generic_get_entries_in_directory(char *path, char **entries);
bool fat_generic_is_file(char *path);
bool fat_generic_is_directory(char *path);
uint32_t fat_generic_get_free_space();
uint32_t fat_generic_get_total_space();

bool fat_generic_create_file(char *path);
bool fat_generic_create_directory(char *path);
bool fat_generic_delete_file(char *path);
bool fat_generic_delete_directory(char *path);
bool fat_generic_rename_file(char *path, char *new_name);
bool fat_generic_rename_directory(char *path, char *new_name);
bool fat_generic_save_to_file(char *path, char *buffer, int size);
bool fat_generic_append_to_file(char *path, char *buffer, int size);

uint8_t fat_generic_copy_filename_to_generic(char *fat_filename, char *generic_filename);
void fat_generic_convert_date_fat_to_generic(fat_directory_entry_date *fat_date, fat_directory_entry_time *fat_time, filesystem_time *generic_time);

#endif