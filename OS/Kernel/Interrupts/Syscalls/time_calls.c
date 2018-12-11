#include "time_calls.h"

void syscall_get_system_clock_call(interrupt_state *state)
{
    state->eax = timer_get_system_clock();
}