#include "memory_calls.h"

void alloc_memory_call(interrupt_state* state)
{
    state->eax = (uint32_t)heap_alloc(state->ebx);
}

void dealloc_memory_call(interrupt_state* state)
{
    heap_dealloc((void*)state->ebx);
}