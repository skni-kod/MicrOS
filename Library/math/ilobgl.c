#include "../math.h"

int ilogbl(long double x)
{
    long double exponent = 0;
    __asm__ (
        "fldt %2 \n" \
        "fxtract \n" \
        "fstpt %1 \n" \
        "fstpt %0"
        : "=m"(exponent), "=m"(x): "m"(x));
    return (int)exponent;
}