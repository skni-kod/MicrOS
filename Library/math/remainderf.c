#include "../math.h"

float remainderf(float x, float y)
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
        "fld %2 \n" \
        "fld %1 \n" \
        "fprem1 \n" \
        "fstp %0"
        : "=m"(x): "m"(x), "m"(y));
    return x;
}