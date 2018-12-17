#ifndef EXCEPTION_STATE_H
#define EXCEPTION_STATE_H

typedef struct exception_state
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

    uint32_t error_code;
    uint32_t eip;
    uint32_t cs;
    uint32_t eflags;
} exception_state;

#endif