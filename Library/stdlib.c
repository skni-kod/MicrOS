#include "stdlib.h"

void sleep(uint32_t ms)
{
    // Temporary implementation, for testing purposes
    uint32_t current_system_clock;
    uint32_t target_system_clock;

    __asm__ ("int $48\n mov %%eax, %0" : "=r" (current_system_clock) :: "%eax");
    target_system_clock = current_system_clock + ms;

    while(current_system_clock < target_system_clock)
    {
        __asm__ ("int $48\n mov %%eax, %0" : "=r" (current_system_clock) :: "%eax");
    }
}