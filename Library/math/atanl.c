#include "../math.h"

long double atanl(long double x)
{
    __asm__ (
        "fldt %1 \n" \
        "fld1 \n" \
        "fpatan \n" \
        "fstpt %0 \n"
        : "=m"(x): "m"(x));
    return x;
}