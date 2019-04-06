#include "../math.h"

float sqrtf(float x)
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
        "fld  %1 \n" \
        "fsqrt \n" \
        "fstp %0"
        : "=m"(x): "m"(x));
    return x;
}