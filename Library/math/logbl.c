#include "../math.h"

long double logbl(long double x)
{
    // FLT_RADIX is guaranteed to be a constant expression.
    #if FLT_RADIX == 2
        return log2l(x);
    #elif
        return log2l(x)/log2l(FLT_RADIX);
    #endif 
}