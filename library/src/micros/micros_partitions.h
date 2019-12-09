#ifndef MICROS_PARTITIONS_H
#define MICROS_PARTITIONS_H

#include "micros_interrupts.h"

#ifdef __cplusplus
extern "C" {
#endif

int micros_partitions_get_count();
void micros_partitions_get_symbols(char *symbols_array);

#ifdef __cplusplus
}
#endif

#endif