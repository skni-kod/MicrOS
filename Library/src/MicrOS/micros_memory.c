#include "micros_memory.h"

void micros_memory_get_physical_memory_stats(micros_physical_memory_stats *stats)
{
    micros_interrupt_1a(0xA0, (uint32_t)stats);
}