#include "../math.h"

float atan2f(float x, float y)
{
    __asm__ (
        "fld %1 \n" \
        "fld %2 \n" \
        "fpatan \n" \
        "fstp %0 \n"
        : "=m"(x): "m"(x), "m"(y));
    return x;
}