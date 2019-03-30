#include "../math.h"

long double sqrtl(long double x)
{
    __asm__ (
        "fldt %1 \n" \
        "fsqrt \n" \
        "fstpt %0"
        : "=m"(x): "m"(x));
    return x;  
}