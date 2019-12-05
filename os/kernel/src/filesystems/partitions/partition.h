#ifndef PARTITION_H
#define PARTITION_H

#include "filesystem_type.h"
#include "device_type.h"

typedef struct partition
{
    char symbol;
    
    filesystem_type type;
    fat_header *header;
    
    device_type device_type;
    int device_number;
    void (*write_on_device)(int device_number, int sector, char *data);
    uint8_t *(*read_from_device)(int device_number, int sector);
    
    fat_header *fat_header_data;
    uint8_t *fat;
    fat_directory_entry *root;

    int first_sector;
    uint32_t fat_length;
    uint32_t directory_length;
} partition;

#endif