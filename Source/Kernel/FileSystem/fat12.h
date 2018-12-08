#ifndef FAT12_H
#define FAT12_H

#define FLOPPY_HEADER_DATA 0xc0007c00

#include "../Drivers/Floppy/floppy_header.h"
#include "../Drivers/Floppy/floppy.h"
#include "../../../Library/stdlib.h"
#include "../../../Library/vector.h"
#include "directory_entry.h"

void fat12_init();
void fat12_load_fat();
void fat12_load_root();
uint8_t* fat12_load_file_from_sector(uint16_t sector, uint16_t* read_sectors_count);
directory_entry* fat12_get_directory_from_path(char* path);
directory_entry* fat12_get_directory_from_chunks(vector* chunks);
uint8_t* fat12_read_file(char* path, uint32_t read_sectors, uint32_t* read_size);

vector* fat12_parse_path(char* path, bool skip_filename);
vector* fat12_list(char* path);
bool fat12_is_entry_valid(directory_entry* entry);
void fat12_merge_filename_and_extension(directory_entry* entry, uint8_t* buffer);

#endif