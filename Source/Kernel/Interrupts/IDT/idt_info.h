#ifndef IDT_INFO_H
#define IDT_INFO_H

#include <stdint.h>

typedef struct idt_info
{
    uint16_t size;
    uint32_t offset;
} __attribute__((packed)) idt_info;

#endif