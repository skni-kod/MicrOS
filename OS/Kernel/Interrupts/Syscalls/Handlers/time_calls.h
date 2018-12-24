#ifndef TIME_CALLS_H
#define TIME_CALLS_H

#include "../../IDT/interrupt_state.h"
#include "../../../Timer/timer.h"

void syscall_get_system_clock_call(interrupt_state *state);

#endif