#include "../math.h"

float atan2f(float x, float y)
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
        "fld %1 \n" \
        "fld %2 \n" \
        "fpatan \n" \
        "fstp %0 \n"
        : "=m"(x): "m"(x), "m"(y));
    return x;
}