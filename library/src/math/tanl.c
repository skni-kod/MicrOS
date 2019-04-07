#include "../math.h"

long double tanl(long double x)
{
    __asm__ (
        "fldt %1 \n" \
        "fptan \n" \
        "fstpt %0 \n" \
        "fstpt %0"
        : "=m"(x): "m"(x));
    return x;
}