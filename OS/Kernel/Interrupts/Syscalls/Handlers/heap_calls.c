#include "heap_calls.h"

void syscall_heap_alloc_memory(interrupt_state *state)
{
    state->registers.eax = (uint32_t)heap_user_alloc(state->registers.ebx, state->registers.ecx);
}

void syscall_heap_realloc_memory(interrupt_state *state)
{
    state->registers.eax = (uint32_t)heap_user_realloc((void *)state->registers.ebx, state->registers.ecx, state->registers.edx);
}

void syscall_heap_dealloc_memory(interrupt_state *state)
{
    heap_user_dealloc((void *)state->registers.ebx);
}

void syscall_heap_get_object_size(interrupt_state *state)
{
    state->registers.eax = (uint32_t)heap_get_object_size((void *)state->registers.ebx);
}

void syscall_heap_verify_integrity(interrupt_state *state)
{
    state->registers.eax = heap_verify_integrity(false);
}

void syscall_heap_get_process_heap(interrupt_state *state)
{
    state->registers.eax = (uint32_t)heap_get_user_heap();
}