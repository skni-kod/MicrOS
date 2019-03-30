#include "../math.h"

float scalblnf(float x, long int n)
{
    // This should be FLX_RADIX based but, the value is 2 on all machines we know of except the IBM 360 and derivatives.
    // So we just could call ldexpf, but it takes int instead of long int so we can copy asm.
    __asm__ (
        "fild %2 \n" \
        "fld %1 \n" \
        "fscale \n" \
        "fstp %0"
        : "=m"(x): "m"(x), "m"(n));
    return x;
}