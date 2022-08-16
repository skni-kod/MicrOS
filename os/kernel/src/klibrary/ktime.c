#include "ktime.h"

void sleep(uint32_t ms)
{
    uint32_t current_system_clock = timer_get_system_clock();
    uint32_t target_system_clock;

    target_system_clock = current_system_clock + ms;

    while (current_system_clock < target_system_clock)
    {
        current_system_clock = timer_get_system_clock();
    }
}