#ifndef FAT_H
#define FAT_H

#define FLOPPY_HEADER_DATA 0xc0007c00

#include <stdlib.h>
#include <kvector.h>
#include "fat_directory_entry.h"
#include "Drivers/Floppy/floppy_header.h"
#include "Drivers/Floppy/floppy.h"
#include "FileSystems/generic/filesystem_file_info.h"
#include "FileSystems/generic/filesystem_directory_info.h"

void fat_init();
void fat_load_fat();
void fat_load_root();
void fat_normalise_filename(char *filename);
void fat_denormalise_filename(char *filename);

uint8_t *fat_load_file_from_sector(uint16_t initial_sector, uint16_t sector_offset, uint16_t sectors_count, uint32_t *read_sectors);
bool fat_read_file(char *path, uint8_t *buffer, uint32_t start_index, uint32_t length);

fat_directory_entry *fat_get_directory_from_path(char *path, uint32_t *read_sectors);
fat_directory_entry *fat_get_directory_from_chunks(kvector *chunks, uint32_t *read_sectors);
fat_directory_entry *fat_get_info(char *path, bool is_directory);

uint32_t fat_get_entries_count_in_directory(char *path);
uint32_t fat_get_entries_in_directory(char *path, char **entries);

kvector *fat_parse_path(char *path);
bool fat_is_entry_valid(fat_directory_entry *entry);
void fat_merge_filename_and_extension(fat_directory_entry *entry, char *buffer);

// Generic filesystem functions
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

uint8_t fat_generic_copy_filename_to_generic(char *fat_filename, char *generic_filename);
void fat_generic_convert_date_fat_to_generic(fat_directory_entry_date *fat_date, fat_directory_entry_time *fat_time, filesystem_time *generic_time);

#endif