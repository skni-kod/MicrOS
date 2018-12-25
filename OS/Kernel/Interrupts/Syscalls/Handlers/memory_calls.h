#ifndef MEMORY_CALLS_H
#define MEMORY_CALLS_H

#include "../../IDT/interrupt_state.h"
#include "../../../Memory/Manager/Heap/heap.h"

void syscall_alloc_memory(interrupt_state *state);
void syscall_realloc_memory(interrupt_state *state);
void syscall_dealloc_memory(interrupt_state *state);
void syscall_get_object_size(interrupt_state *state);
void syscall_verify_heap_integrity(interrupt_state *state);
void syscall_get_process_heap(interrupt_state *state);

#endif