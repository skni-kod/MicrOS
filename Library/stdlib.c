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

char *itoa(int input, char *buffer, int base)
{
    int8_t index = 0;
    int8_t kappa = 0;

    int8_t sign = 0;
    if (input < 0)
    {
        input *= -1;
        sign = 1;
    }

    do
    {
        kappa = input % base;
        buffer[index] = (kappa >= 10 ? kappa - 10 + 'a' : kappa +  '0');

        input /= base;

        ++index;
    } while (input > 0);

    if (sign)
    {
        buffer[index] = '-';
        ++index;
    }

    for (int i = 0; i < index / 2; i++)
    {
        char tmp = buffer[i];
        buffer[i] = buffer[index - i - 1];
        buffer[index - i - 1] = tmp;
    }

    buffer[index] = '\0';

    return buffer;
}