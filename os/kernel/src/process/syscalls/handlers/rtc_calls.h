#ifndef RTC_CALLS_H
#define RTC_CALLS_H

#include "cpu/idt/interrupt_state.h"
#include "Drivers/RTC/rtc.h"

void syscall_rtc_get_time(interrupt_state *state);

#endif