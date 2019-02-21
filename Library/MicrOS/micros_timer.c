#include "micros_timer.h"

uint32_t micros_timer_get_system_clock()
{
    return micros_interrupt_0a(0x60);
}