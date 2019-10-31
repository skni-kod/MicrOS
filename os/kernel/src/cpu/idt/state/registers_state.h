#ifndef REGISTERS_STATE_H
#define REGISTERS_STATE_H

typedef struct registers_state
{
    uint32_t edi;
    uint32_t esi;
    uint32_t ebp;
    uint32_t esp_unused;
    uint32_t ebx;
    uint32_t edx;
    uint32_t ecx;
    uint32_t eax;
} registers_state;

#endif