#include "../math.h"

double frexp(double x, int* exp)
{
    if(x == 0)
    {
        *exp = 0;
        return 0;
    }

    double exponent = 0;
    __asm__ (
        "fldl %2 \n" \
        "fxtract \n" \
        "fstpl %1 \n" \
        "fstpl %0"
        : "=m"(exponent), "=m"(x): "m"(x));

        // x is positive
        if(x > 0)
        {
            for(;;)
            {
                if(x >= 0.5 && x < 1)
                {
                    break;
                }
                x = x/2;
                exponent++;
            }
        }
        // x is negative
        else if(x < 0)
        {
            for(;;)
            {
                if(x <= -0.5 && x > -1)
                {
                    break;
                }
                x = x/2;
                exponent++;
            }
        }
        *exp = (int)exponent;
    return x;
}