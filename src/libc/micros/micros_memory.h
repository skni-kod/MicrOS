#ifndef MICROS_MEMORY_H
#define MICROS_MEMORY_H

#include "micros_interrupts.h"

//! Structure containing memory statistics
typedef struct micros_physical_memory_stats
{
    //! Free entries (sector of 512 bytes each)
    uint32_t free_entries;
    
    //! Reserved entries (sector of 512 bytes each)
    uint32_t reserved_entries;
    
    //! Allocated entries (sector of 512 bytes each)
    uint32_t allocated_entries;
} micros_physical_memory_stats;

#ifdef __cplusplus
extern "C" {
#endif

//! Retrieves statistics about memory usage.
/*!
    Retrieves statistics about memory usage and fills passed structure.
    \param stats Structure where statistics will be stored.
*/
void micros_memory_get_physical_memory_stats(micros_physical_memory_stats *stats);

#ifdef __cplusplus
}
#endif

#endif