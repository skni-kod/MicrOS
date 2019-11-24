#include "partitions_calls.h"

void syscall_partitions_get_count(interrupt_state *state)
{
    state->registers.eax = partitions_get_count();
}