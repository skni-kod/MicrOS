#ifndef MICROS_PARTITIONS_H
#define MICROS_PARTITIONS_H

#include "micros_interrupts.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct micros_partition_info
{
    char device_name[64];
    uint32_t device_type;
} __attribute__((packed)) micros_partition_info;

int micros_partitions_get_count();
void micros_partitions_get_symbols(char *symbols_array);
void micros_partitions_get_info(char symbol, micros_partition_info *partition_info);

#ifdef __cplusplus
}
#endif

#endif