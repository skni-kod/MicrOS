#include "../math.h"

long double fabsl(long double x)
{
    __asm__ (
        "fldt %1 \n" \
        "fabs \n " \
        "fstpt %0"
        : "=m"(x): "m"(x));
    return x;
}