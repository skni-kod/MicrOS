#include "../stdlib.h"

int atoi(const char *string)
{

    size_t len = strlen(string);
    int value = 0;
    int position = 1;

    char sign = 1;
    char lengthOffset = 0;
    if (string[0] == '-')
    {
        sign = -1;
        lengthOffset = 1;
    }

    for (int8_t i = len - 1; i >= lengthOffset; --i)
    {
        if(string[i] >= '0' && string[i] <= '9'){
            value += (string[i] - '0') * position;
            position *= 10;
        }else{
            value = 0;
            position = 1;
        }
    }

    return value * sign;
}