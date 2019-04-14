#include "../math.h"

float fabsf(float x)
{
    __asm__ (
        "fld %1 \n" \
        "fabs \n" \
        "fstp %0"
        : "=m"(x): "m"(x));
    return x;
}