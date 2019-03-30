#include "../math.h"

long double atan2l(long double x, long double y)
{
    __asm__ (
        "fldt %1 \n" \
        "fldt %2 \n" \
        "fpatan \n" \
        "fstpt %0 \n"
        : "=m"(x): "m"(x), "m"(y));
    return x;
}