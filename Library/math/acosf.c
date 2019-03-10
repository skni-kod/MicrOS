#include "../math.h"

float acosf(float x)
{
    float two = 2;
    float asin_x = asinf(x);
    __asm__ (
        "fld %1 \n" \
        "fldpi \n" \
        "fdivp \n" \
        "fld %2 \n" \
        "fsubrp \n" \
        "fstp %0"
        : "=m"(x): "m"(two), "m"(asin_x));
    return x;
}