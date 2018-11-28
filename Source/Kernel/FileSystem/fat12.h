#ifndef FAT12_H
#define FAT12_H

#define FLOPPY_HEADER_DATA 0xc0007c00

#include "../Drivers/Floppy/floppy_header.h"
#include "../Drivers/Floppy/floppy.h"
#include "../../../Library/stdlib.h"

void fat12_init();
void fat12_load_fat();
void fat12_load_root();
uint16_t fat12_read_sector_value(uint32_t sector_number);

#endif