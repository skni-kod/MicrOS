#include "../stdlib.h"

char *itoa(int input, char *buffer, int base)
{
    int idx = 0;
    static const char lowercase_table[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};

    bool negative = input < 0;
    if (negative)
        input *= -1;

    // put number to buffer in reverse order
    do
    {
        size_t digit = (input % base);
        buffer[idx++] = lowercase_table[digit];

        input /= base;
    } while (input);

    //put sign
    if (negative)
    {
        buffer[idx++] = '-';
    }

    size_t size = idx;
    idx--;

    // revert string
    for (size_t i = 0; i < size / 2; i++)
    {
        char tmp = buffer[i];
        buffer[i] = buffer[idx];
        buffer[idx] = tmp;

        idx--;
    }

    buffer[size] = '\0';

    return buffer;
}