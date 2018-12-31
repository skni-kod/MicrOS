#include "memory_calls.h"

void syscall_memory_get_physical_memory_stats(interrupt_state *state)
{
    physical_memory_get_stats((physical_memory_stats *)state->registers.ebx);
}