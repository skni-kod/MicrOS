#ifndef TIME_CALLS_H
#define TIME_CALLS_H

#include "cpu/idt/state/interrupt_state.h"
#include "cpu/timer/timer.h"

void syscall_timer_get_system_clock(interrupt_state *state);

#endif