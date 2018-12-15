#include "memory_calls.h"

void syscall_alloc_memory_call(interrupt_state *state)
{
    state->eax = (uint32_t)heap_alloc(state->ebx, state->ecx);
}

void syscall_dealloc_memory_call(interrupt_state *state)
{
    heap_dealloc((void *)state->ebx);
}