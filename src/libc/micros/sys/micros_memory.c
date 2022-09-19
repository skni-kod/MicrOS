#include "micros_memory.h"

void micros_memory_get_physical_memory_stats(micros_physical_memory_stats *stats)
{
#ifndef __libk 

    micros_interrupt_1a(0x0A00, (uint32_t)stats);

#else

#endif
}