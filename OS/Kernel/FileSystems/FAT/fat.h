#ifndef FAT_H
#define FAT_H

#define FLOPPY_HEADER_DATA 0xc0007c00

#include "../Drivers/Floppy/floppy_header.h"
#include "../Drivers/Floppy/floppy.h"
#include "kvector.h"
#include <stdlib.h>
#include "fat_directory_entry.h"

void fat_init();
void fat_load_fat();
void fat_load_root();
void fat_normalise_filename(char *filename);
uint8_t *fat_load_file_from_sector(uint16_t sector, uint16_t *read_sectors_count);
fat_directory_entry *fat_get_directory_from_path(char *path);
fat_directory_entry *fat_get_directory_from_chunks(kvector *chunks);
uint8_t *fat_read_file(char *path, uint16_t *read_sectors, uint16_t *read_size);
fat_directory_entry *fat_get_info(char *path, bool is_directory);

kvector *fat_parse_path(char *path);
kvector *fat_list(char *path);
bool fat_is_entry_valid(fat_directory_entry *entry);
void fat_merge_filename_and_extension(fat_directory_entry *entry, uint8_t *buffer);

#endif