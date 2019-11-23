#ifndef RTC_CALLS_H
#define RTC_CALLS_H

#include "cpu/idt/state/interrupt_state.h"
#include "drivers/rtc/rtc.h"

void syscall_rtc_get_time(interrupt_state *state);
void syscall_rtc_set_time(interrupt_state *state);

#endif