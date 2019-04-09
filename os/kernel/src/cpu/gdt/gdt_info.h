#ifndef GDT_DESCRIPTION_H
#define GDT_DESCRIPTION_H

#include <stdint.h>

typedef struct gdt_info
{
    uint16_t size;
    uint32_t offset;
} __attribute__((packed)) gdt_info;

#endif