#ifndef FAT_HEADER
#define FAT_HEADER

#include <stdint.h>

typedef struct fat_header
{
    // Bios Parameter Block
    uint32_t jump_signature : 24;
    char oem_identifier[8];
    uint16_t bytes_per_sector;
    uint8_t sectors_per_cluster;
    uint16_t reserved_sectors;
    uint8_t fat_count;
    uint16_t directory_entries;
    uint16_t total_sectors;
    uint8_t media_descriptor_type;
    uint16_t sectors_per_fat;
    uint16_t sectors_per_track;
    uint16_t heads;
    uint32_t hidden_sectors;
    uint32_t large_sectors;

    // Extended Boot Record
    uint8_t drive_number;
    uint8_t flags;
    uint8_t signature;
    uint32_t volume_id;
    char volume_label[11];
    char system_identifier[8];
} __attribute__((packed)) fat_header;

#endif