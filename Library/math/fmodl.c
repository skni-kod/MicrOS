#include "../math.h"

long double fmodl(long double numer, long double denom)
{
    if(denom == 0)
    {
        errno = EDOM;
        return 0;
    }
    __asm__ (
        "fldt %2 \n" \
        "fldt %1 \n" \
        "fprem \n" \
        "fstpt %0"
        : "=m"(numer): "m"(numer), "m"(denom));
    return numer;
}