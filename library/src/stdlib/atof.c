#include "../stdlib.h"

double atof(const char* string)
{
    size_t len = strlen(string);
    int intValue = 0;
    int decValue = 0;

    char decPlaces = 0;

    uint16_t position = 1;

    char sign = 1;
    char lengthOffset = 0;

    char dot = 0;
    double value = 1;

    if (*string == '-')
    {
        sign = -1;
        lengthOffset = 1;
    }

    for (size_t i = len-1; i >= lengthOffset; --i)
    {

        if (string[i] != '.') {
            if (dot == 1) {
                intValue += (string[i] - 48) * position;
                position *= 10;
            } else {
                decValue += (string[i] - 48) * position;
                position *= 10;
                decPlaces++;
            }
        } else {
            position = 1;
            dot = 1;
        }
    }

    value = pow(10, (-1) * decPlaces);

    value *= decValue;
    value += intValue;
    value *= sign;

    return value;

}