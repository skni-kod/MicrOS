#include "../math.h"

double remquo(double numer, double denom, int* quot)
{
    if(denom == 0)
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
        : "=m"(numer): "m"(numer), "m"(denom));

    fexcept_t except = __FPU_read_status_word();
    *quot = except.condition_code_0 << 2 | except.condition_code_3 << 1 | except.condition_code_1;

    return numer;
}