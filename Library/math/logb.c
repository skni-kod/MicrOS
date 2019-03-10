#include "../math.h"

double logb(double x)
{
    if(FLT_RADIX == 2)
    {
        return log2(x);
    }
    else
    {
        return log2(x)/log2(FLT_RADIX);
    }   
}