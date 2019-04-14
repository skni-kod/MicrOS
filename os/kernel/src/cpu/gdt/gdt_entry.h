#ifndef GDT_ENTRY_H
#define GDT_ENTRY_H

#include <stdint.h>

typedef struct gdt_entry
{
    uint16_t limit_0_15;
    uint16_t base_0_15;
    uint8_t base_16_23;

    uint8_t accessed : 1;
    uint8_t read_write : 1;
    uint8_t direction : 1;
    uint8_t executable : 1;
    uint8_t reserved_1 : 1;
    uint8_t privilege_level : 2;
    uint8_t present : 1;

    uint8_t limit_16_19 : 4;
    uint8_t reserved_2 : 2;
    uint8_t size : 1;
    uint8_t granularity : 1;

    uint8_t base_24_31;

} __attribute__((packed)) gdt_entry;

#endif