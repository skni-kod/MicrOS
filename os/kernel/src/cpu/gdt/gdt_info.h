#ifndef GDT_DESCRIPTION_H
#define GDT_DESCRIPTION_H
#pragma pack(1)

#include <stdint.h>

typedef struct gdt_info
{
    uint16_t size;
    uint32_t offset;
} gdt_info;

#endif