#include "../math.h"

long double remaindel(long double x,long double y)
{
    if(y == 0)
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
        "fldt %2 \n" \
        "fldt %1 \n" \
        "fprem1 \n" \
        "fstpt %0"
        : "=m"(x): "m"(x), "m"(y));
    return x;
}