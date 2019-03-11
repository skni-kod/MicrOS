#include "../math.h"

double logb(double x)
{
    // FLT_RADIX is guaranteed to be a constant expression.
    #if FLT_RADIX == 2
        return log2(x);
    #elif
        return log2(x)/log2(FLT_RADIX);
    #endif 
}