#ifndef PARTITION_H
#define PARTITION_H

#include "filesystem_type.h"

typedef struct partition
{
    char symbol;
    
    filesystem_type type;
    floppy_header *header;
    
    
} partition;

#endif