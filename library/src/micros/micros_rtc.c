#include "micros_rtc.h"

void micros_rtc_read_time(micros_rtc_time *time)
{
    micros_interrupt_1a(0x40, (uint32_t)time);
}

void micros_rtc_set_time(micros_rtc_time *time)
{
    micros_interrupt_1a(0x41, (uint32_t)time);
}