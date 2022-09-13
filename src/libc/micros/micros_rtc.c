#include "micros_rtc.h"

void micros_rtc_read_time(micros_rtc_time *time)
{
#ifndef __libk 

    micros_interrupt_1a(0x0400, (uint32_t)time);

#else

#endif
}

void micros_rtc_set_time(micros_rtc_time *time)
{
#ifndef __libk 

    micros_interrupt_1a(0x0401, (uint32_t)time);

#else

#endif
}