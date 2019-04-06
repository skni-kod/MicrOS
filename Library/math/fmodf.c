#include "../math.h"

float fmodf(float numer, float denom)
{
    if(denom == 0)
    {
        // domain error
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
        "fld %2 \n" \
        "fld %1 \n" \
        "fprem \n" \
        "fstp %0"
        : "=m"(numer): "m"(numer), "m"(denom));
    return numer;
}