#include "../math.h"

long double ldexpl(long double x, int exp)
{
    __asm__ (
        "fild %2 \n" \
        "fldt %1 \n" \
        "fscale \n" \
        "fstpt %0"
        : "=m"(x): "m"(x), "m"(exp));
    return x;
}