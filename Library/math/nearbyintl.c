#include "../math.h"

long double nearbyintl(long double x)
{
    __asm__ (
        "fldt %1 \n" \
        "frndint \n" \
        "fstpt %0"
        : "=m"(x): "m"(x));
    return x;
}