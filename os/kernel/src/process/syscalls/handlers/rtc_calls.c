#include "rtc_calls.h"

void syscall_rtc_get_time(interrupt_state *state)
{
    rtc_time *time = (rtc_time *)state->registers.ebx;
    rtc_read(time);
}

void syscall_rtc_set_time(interrupt_state *state)
{
    rtc_time *time = (rtc_time *)state->registers.ebx;
    rtc_set(time);
}