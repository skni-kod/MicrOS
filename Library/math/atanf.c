#include "../math.h"

float atanf(float x)
{
    __asm__ (
        "fld %1 \n" \
        "fld1 \n" \
        "fpatan \n" \
        "fstp %0 \n"
        : "=m"(x): "m"(x));
    return x;
}