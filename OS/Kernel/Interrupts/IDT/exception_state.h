#ifndef EXCEPTION_STATE_H
#define EXCEPTION_STATE_H

#include "registers_state.h"
#include "fpu_state.h"

typedef struct exception_state
{
    registers_state registers;
    fpu_state fpu_state;

    uint32_t interrupt_number;

    uint32_t error_code;
    uint32_t eip;
    uint32_t cs;
    uint32_t eflags;
} exception_state;

#endif