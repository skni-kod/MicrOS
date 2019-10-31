#include "../math.h"

long double sqrtl(long double x)
{
    if(x < 0)
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
        "fsqrt \n" \
        "fstpt %0"
        : "=m"(x): "m"(x));
    return x;  
}