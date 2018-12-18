#ifndef INTERRUPT_STATE_H
#define INTERRUPT_STATE_H

#include <stdint.h>
#include "registers_state.h"

typedef struct interrupt_state
{
    registers_state registers;

    uint32_t interrupt_number;

    uint32_t eip;
    uint32_t cs;
    uint32_t eflags;
} interrupt_state;

#endif