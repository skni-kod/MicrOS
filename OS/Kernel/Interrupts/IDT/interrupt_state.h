#ifndef INTERRUPT_STATE_H
#define INTERRUPT_STATE_H

#include <stdint.h>
#include "registers_state.h"
#include "fpu_state.h"

typedef struct interrupt_state
{
    fpu_state fpu_state;
    registers_state registers;

    uint32_t interrupt_number;

    uint32_t eip;
    uint32_t cs;
    uint32_t eflags;
    uint32_t esp;
    uint32_t ss;
} interrupt_state;

#endif