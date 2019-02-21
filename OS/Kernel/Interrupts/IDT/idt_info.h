#ifndef IDT_INFO_H
#define IDT_INFO_H
#pragma pack(1)

#include <stdint.h>

typedef struct idt_info
{
    uint16_t size;
    uint32_t offset;
} idt_info;

#endif