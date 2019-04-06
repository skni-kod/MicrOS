#include "../math.h"

float fmodf(float numer, float denom)
{
    if(denom == 0)
    {
        errno = EDOM;
        return 0;
    }
    __asm__ (
        "fld %2 \n" \
        "fld %1 \n" \
        "fprem \n" \
        "fstp %0"
        : "=m"(numer): "m"(numer), "m"(denom));
    return numer;
}