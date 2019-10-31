#ifndef SIGNAL_PARAMS_H
#define SIGNAL_PARAMS_H

#include "cpu/idt/state/interrupt_state.h"
#include "cpu/idt/state/fpu_state.h"

typedef struct signal_params
{
    uint32_t interrupt_number;
    uint32_t cr2;

    registers_state registers;
    fpu_state fpu_state;

    uint32_t eip;
    uint32_t cs;
    uint32_t eflags;
    uint32_t esp;
    uint32_t ss;
} signal_params;

#endif