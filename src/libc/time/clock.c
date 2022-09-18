#include "../time.h"

clock_t clock()
{
    return micros_timer_get_system_clock();
}