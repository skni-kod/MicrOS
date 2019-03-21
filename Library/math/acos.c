#include "../math.h"

double acos(double x)
{
    double two = 2;
    double asin_x = asin(x);
    __asm__ (
        "fldl %1 \n" \
        "fldpi \n" \
        "fdivp \n" \
        "fldl %2 \n" \
        "fsubrp \n" \
        "fstpl %0"
        : "=m"(x): "m"(two), "m"(asin_x));
    return x;
}