#ifndef EXCEPTION_STATE_H
#define EXCEPTION_STATE_H

#include "registers_state.h"
#include "fpu_state.h"

typedef struct exception_state
{
    uint64_t idtr;
    uint64_t gdtr;
    uint32_t gs;
    uint32_t fs;
    uint32_t es;
    uint32_t cr4;
    uint32_t cr3;
    uint32_t cr2;
    uint32_t cr0;
    uint32_t ds;

    registers_state registers;
    fpu_state fpu_state;

    uint32_t interrupt_number;

    uint32_t error_code;
    uint32_t eip;
    uint32_t cs;
    uint32_t eflags;
    uint32_t esp;
    uint32_t ss;
} exception_state;

#endif