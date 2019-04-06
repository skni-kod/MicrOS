#include "../math.h"

long double frexpl(long double x, int* exp)
{
    if(x == 0)
    {
        *exp = 0;
        return 0;
    }

    long double exponent = 0;
    __asm__ (
        "fldt %2 \n" \
        "fxtract \n" \
        "fstpt %1 \n" \
        "fstpt %0"
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