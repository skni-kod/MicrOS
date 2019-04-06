#include "../math.h"

double atan(double x)
{
    if(x < -M_PI/2 || x > M_PI/2)
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
        "fldl  %1 \n" \
        "fld1 \n" \
        "fpatan \n" \
        "fstpl %0 \n"
        : "=m"(x): "m"(x));
    return x;
}