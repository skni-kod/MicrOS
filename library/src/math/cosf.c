#include "../math.h"

float cosf(float x)
{
    __asm__ (
        "fld %1 \n" \
        "fcos \n" \
        "fstp %0"
        : "=m"(x): "m"(x));
    return x;
}