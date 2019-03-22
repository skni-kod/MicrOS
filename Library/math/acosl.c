#include "../math.h"

long double acosl(long double x)
{
    if(x < -1 || x > 1)
    {
        if(_math_errhandling == MATH_ERRNO)
        {
            errno = EDOM;
        }
        else if(_math_errhandling == MATH_ERREXCEPT)
        {
            feraiseexcept(FE_INVALID);  
        }
        return 0;
    }
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