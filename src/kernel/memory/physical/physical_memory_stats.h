#ifndef PHYSICAL_MEMORY_STATS_H
#define PHYSICAL_MEMORY_STATS_H

typedef struct physical_memory_stats
{
    uint32_t free_entries;
    uint32_t reserved_entries;
    uint32_t allocated_entries;
} physical_memory_stats;

#endif