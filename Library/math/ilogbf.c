#include "../math.h"

int ilogbf(float x)
{
    float exponent = 0;
    __asm__ (
        "fldl %2 \n" \
        "fxtract \n" \
        "fstpl %1 \n" \
        "fstpl %0"
        : "=m"(exponent), "=m"(x): "m"(x));
    return (int)exponent;
}