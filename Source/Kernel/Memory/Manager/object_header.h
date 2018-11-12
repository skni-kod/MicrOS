#ifndef OBJECT_HEADER_H
#define OBJECT_HEADER_H

#include <stdint.h>

typedef struct object_header
{
    uint32_t size : 31;
    uint8_t free : 1;

    struct object_header* next;
} object_header;

#endif