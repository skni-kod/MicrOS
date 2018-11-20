#ifndef INTERRUPT_STATE_H
#define INTERRUPT_STATE_H

#include <stdint.h>

typedef struct interrupt_state
{
    uint32_t eax;
    uint32_t ebx;
    uint32_t ecx;
    uint32_t edx;
    uint32_t esp;
    uint32_t ebp;
    uint32_t esi;
    uint32_t edi;

    uint32_t interrupt_number;
} interrupt_state;

#endif