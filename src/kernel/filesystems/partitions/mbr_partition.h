#ifndef MBR_PARTITION_H
#define MBR_PARTITION_H

typedef struct mbr_partition
{
    uint8_t status;
    uint8_t first_head;
    uint8_t first_sector : 6;
    uint16_t first_track : 10;
    uint8_t type;
    uint8_t last_head;
    uint8_t last_sector : 6;
    uint16_t last_track : 10;
    uint32_t first_sector_lba;
    uint32_t sectors_count;
} __attribute__((packed)) mbr_partition;

#endif