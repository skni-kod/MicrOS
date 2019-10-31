#ifndef MICROS_MEMORY_H
#define MICROS_MEMORY_H

#include "micros_interrupts.h"

typedef struct micros_physical_memory_stats
{
    uint32_t free_entries;
    uint32_t reserved_entries;
    uint32_t allocated_entries;
} micros_physical_memory_stats;

#ifdef __cplusplus
extern "C" {
#endif

void micros_memory_get_physical_memory_stats(micros_physical_memory_stats *stats);

#ifdef __cplusplus
}
#endif

#endif