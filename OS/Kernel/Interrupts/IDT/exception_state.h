#ifndef EXCEPTION_STATE_H
#define EXCEPTION_STATE_H

typedef struct exception_state
{
    registers_state registers;

    uint32_t interrupt_number;

    uint32_t error_code;
    uint32_t eip;
    uint32_t cs;
    uint32_t eflags;
} exception_state;

#endif