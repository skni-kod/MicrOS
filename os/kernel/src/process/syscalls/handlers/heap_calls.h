#ifndef HEAP_CALLS_H
#define HEAP_CALLS_H

#include "cpu/idt/state/interrupt_state.h"
#include "memory/heap/heap.h"

void syscall_heap_alloc_memory(interrupt_state *state);
void syscall_heap_realloc_memory(interrupt_state *state);
void syscall_heap_dealloc_memory(interrupt_state *state);
void syscall_heap_get_object_size(interrupt_state *state);
void syscall_heap_verify_integrity(interrupt_state *state);
void syscall_heap_get_process_heap(interrupt_state *state);

#endif