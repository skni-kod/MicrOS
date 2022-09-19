#include "micros_timer.h"

uint32_t micros_timer_get_system_clock()
{
#ifndef __libk 

#ifndef __libk
    return micros_interrupt_0a(0x0600);
#else

#endif

#else

#endif
}