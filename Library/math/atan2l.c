#include "../math.h"

long double atan2l(long double x, long double y)
{
    if(x == 0 && y == 0)
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
    __asm__ (
        "fldt %1 \n" \
        "fldt %2 \n" \
        "fpatan \n" \
        "fstpt %0 \n"
        : "=m"(x): "m"(x), "m"(y));
    return x;
}