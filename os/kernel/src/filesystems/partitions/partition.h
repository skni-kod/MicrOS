#ifndef PARTITION_H
#define PARTITION_H

#include "filesystem_type.h"
#include "device_type.h"

typedef struct partition
{
    char symbol;
    
    filesystem_type type;
    floppy_header *header;
    
    device_type device_type;
    int device_number;
    
    floppy_header *fat_header_data;
    uint8_t *fat;
    fat_directory_entry *root;

    uint32_t fat_length;
    uint32_t directory_length;
} partition;

#endif