#include "../math.h"

float log2f(float x)
{
    // domain error
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
    // pole error
    if(x == 0)
    {
        if(_math_errhandling == MATH_ERRNO)
        {
            errno = ERANGE;
        }
        else if(_math_errhandling == MATH_ERREXCEPT)
        {
            feraiseexcept(FE_DIVBYZERO);  
        }
        return -__math_INFf();
    }
    __asm__ (
        "fld1 \n" \
        "fld %1 \n" \
        "fyl2x \n" \
        "fstp %0 \n"
        : "=m"(x): "m"(x));
    return x;
}