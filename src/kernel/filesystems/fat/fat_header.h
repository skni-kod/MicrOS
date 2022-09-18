#ifndef FAT_HEADER
#define FAT_HEADER

#include <stdint.h>

typedef struct fat_header
{
    // Bios Parameter Block
    uint32_t jump_signature : 24;//0x9049eb
    char oem_identifier[8];//MICROS  
    uint16_t bytes_per_sector;//512
    uint8_t sectors_per_cluster;//1
    uint16_t reserved_sectors;//1
    uint8_t fat_count;//2
    uint16_t directory_entries;//224
    uint16_t total_sectors;//2880
    uint8_t media_descriptor_type;//240
    uint16_t sectors_per_fat;//9
    uint16_t sectors_per_track;//18
    uint16_t heads;//2
    uint32_t hidden_sectors;//0
    uint32_t large_sectors;//0

    // Extended Boot Record
    uint8_t drive_number;//0
    uint8_t flags;//0
    uint8_t signature;//41
    uint32_t volume_id;//0x12345678
    char volume_label[11];//SYSTEM PART
    char system_identifier[8];//FAT 12   
} __attribute__((packed)) fat_header;

#endif