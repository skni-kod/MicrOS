#include "../math.h"

float asinf(float x)
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
    __asm__ (
        "fld %1 \n" \
        "fld %1 \n" \
        "fmulp \n" \
        "fld1 \n" \
        "fsubp \n" \
        "fsqrt \n" \
        "fld %1 \n" \
        "fdivp \n" \
        "fld1 \n" \
        "fpatan \n" \
        "fstp %0"
        : "=m"(x): "m"(x));
    return x;
}