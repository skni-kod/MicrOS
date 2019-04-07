#include "../math.h"

long double fmal(long double  x, long double  y, long double  z)
{
    __asm__ (
        "fldt %3 \n" \
        "fldt %2 \n" \
        "fldt %1 \n" \
        "fmulp \n" \
        "faddp \n" \
        "fstpt %0"
        : "=m"(x): "m"(x), "m"(y), "m"(z));
    return x;
}