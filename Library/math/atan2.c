#include "../math.h"

double atan2(double x, double y)
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
        "fldl %1 \n" \
        "fldl %2 \n" \
        "fpatan \n" \
        "fstpl %0 \n"
        : "=m"(x): "m"(x), "m"(y));
    return x;
}