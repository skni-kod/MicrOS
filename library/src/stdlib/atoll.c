#include "../stdlib.h"

long long int atoll(const char *string)
{
    size_t len = strlen(string);
    long long int value = 0;
    int position = 1;

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