#include "../math.h"

float logbf(float x)
{
    if(FLT_RADIX == 2)
    {
        return log2f(x);
    }
    else
    {
        return log2f(x)/log2f(FLT_RADIX);
    }  
}