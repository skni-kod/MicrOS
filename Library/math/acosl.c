#include "../math.h"

long double acosl(long double x)
{
    long double two = 2;
    long double asin_x = asinl(x);
    __asm__ (
        "fldt %1 \n" \
        "fldpi \n" \
        "fdivp \n" \
        "fldt %2 \n" \
        "fsubrp \n" \
        "fstpt %0"
        : "=m"(x): "m"(two), "m"(asin_x));
    return x;
}