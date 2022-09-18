#ifndef PARTITION_INFO_H
#define PARTITION_INFO_H

typedef struct partition_info
{
    char device_name[64];
    uint32_t device_type;
} __attribute__((packed)) partition_info;

#endif