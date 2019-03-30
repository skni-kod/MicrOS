#include "../math.h"

double remainder(double x, double y)
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
        "fldl %2 \n" \
        "fldl %1 \n" \
        "fprem1 \n" \
        "fstpl %0"
        : "=m"(x): "m"(x), "m"(y));
    return x;
}