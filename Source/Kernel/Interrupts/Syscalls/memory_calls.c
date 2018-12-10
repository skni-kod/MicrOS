#include "memory_calls.h"

void alloc_memory_call(interrupt_state* state)
{
    state->eax = heap_alloc(state->ebx);
}

void dealloc_memory_call(interrupt_state* state)
{
    heap_dealloc(state->ebx);
}