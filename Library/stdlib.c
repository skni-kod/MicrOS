#include "stdlib.h"
#include "string.h"

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

int32_t atoi(const char *string)
{
    size_t len = strlen(string);
    int32_t value = 0;
    int32_t position = 1;

    char sign = 1;
    char lengthOffset = 0;
    if (*string == '-')
    {
        sign = -1;
        lengthOffset = 1;
    }

    for (int8_t i = len - 1; i >= lengthOffset; --i)
    {
        value += (string[i] - '0') * position;
        position *= 10;
    }

    return value * sign;
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