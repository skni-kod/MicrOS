#include "../math.h"

float logbf(float x)
{
    // FLT_RADIX is guaranteed to be a constant expression.
    #if FLT_RADIX == 2
        return log2f(x);
    #elif
        return log2f(x)/log2f(FLT_RADIX);
    #endif
}