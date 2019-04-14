#include "../math.h"

long double sinl(long double x)
{
    __asm__ (
        "fldt %1 \n" \
        "fsin \n" \
        "fstpt %0"
        : "=m"(x): "m"(x));
    return x;
}
