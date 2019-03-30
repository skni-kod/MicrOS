#include "../math.h"

double fmod(double numer, double denom)
{
    if(denom == 0)
    {
        errno = EDOM;
        return 0;
    }
    __asm__ (
        "fldl %2 \n" \
        "fldl %1 \n" \
        "fprem \n" \
        "fstpl %0"
        : "=m"(numer): "m"(numer), "m"(denom));
    return numer;
}