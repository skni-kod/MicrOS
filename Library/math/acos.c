#include "../math.h"

double acos(double x)
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
    double two = 2;
    double asin_x = asin(x);
    __asm__ (
        "fldl %1 \n" \
        "fldpi \n" \
        "fdivp \n" \
        "fldl %2 \n" \
        "fsubrp \n" \
        "fstpl %0"
        : "=m"(x): "m"(two), "m"(asin_x));
    return x;
}