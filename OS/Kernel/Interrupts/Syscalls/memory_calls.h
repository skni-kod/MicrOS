#ifndef MEMORY_CALLS_H
#define MEMORY_CALLS_H

#include "../IDT/interrupt_state.h"
#include "../../Memory/Manager/Heap/heap.h"

void syscall_alloc_memory_call(interrupt_state *state);
void syscall_dealloc_memory_call(interrupt_state *state);

#endif