#include "micros_partitions.h"

int micros_partitions_get_count()
{
    return micros_interrupt_0a(0xB0);
}

void micros_partitions_get_symbols(char *symbols_array)
{
    micros_interrupt_1a(0xB1, (uint32_t)symbols_array);
}

void micros_partitions_get_info(char symbol, micros_partition_info *partition_info)
{
    micros_interrupt_2a(0xB2, (uint32_t)symbol, (uint32_t)partition_info);
}