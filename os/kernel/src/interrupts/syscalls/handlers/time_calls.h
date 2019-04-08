#ifndef TIME_CALLS_H
#define TIME_CALLS_H

#include "Interrupts/IDT/interrupt_state.h"
#include "Timer/timer.h"

void syscall_timer_get_system_clock(interrupt_state *state);

#endif