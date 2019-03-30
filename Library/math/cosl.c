#include "../math.h"

long double cosl(long double x)
{
    __asm__ (
        "fldt %1 \n" \
        "fcos \n" \
        "fstpt %0"
        : "=m"(x): "m"(x));
    return x;
}