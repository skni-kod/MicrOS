#include "../math.h"

double log10(double x)
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
        return -__math_INF();
    }
    return log2(x)/log2(10.);
}