#ifndef MBR_H
#define MBR_H

#include "mbr_partition.h"

typedef struct mbr
{
    uint8_t bootloader[446];
    mbr_partition partitions[4];
    uint16_t end_sign;
} mbr;

#endif