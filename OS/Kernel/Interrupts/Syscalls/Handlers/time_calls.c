#include "time_calls.h"

void syscall_timer_get_system_clock(interrupt_state *state)
{
    state->registers.eax = timer_get_system_clock();
}