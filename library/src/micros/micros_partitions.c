#include "micros_partitions.h"

int micros_partitions_get_count()
{
    return micros_interrupt_0a(0xB0);
}