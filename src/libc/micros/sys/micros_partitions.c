#include "micros_partitions.h"

int micros_partitions_get_count()
{
#ifndef __libk 

    return micros_interrupt_0a(0x0B00);

#else

#endif
}

void micros_partitions_get_symbols(char *symbols_array)
{
#ifndef __libk 

    micros_interrupt_1a(0x0B01, (uint32_t)symbols_array);

#else

#endif
}

void micros_partitions_get_info(char symbol, micros_partition_info *partition_info)
{
#ifndef __libk 

    micros_interrupt_2a(0x0B02, (uint32_t)symbol, (uint32_t)partition_info);

#else

#endif
}