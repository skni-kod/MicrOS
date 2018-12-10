#include "time_calls.h"

void get_system_clock_call(interrupt_state* state)
{
    state->eax = timer_get_system_clock();
}