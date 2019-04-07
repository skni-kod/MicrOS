#include "../math.h"

double fmod(double numer, double denom)
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
        "fldl %2 \n" \
        "fldl %1 \n" \
        "fprem \n" \
        "fstpl %0"
        : "=m"(numer): "m"(numer), "m"(denom));
    return numer;
}