#include "../math.h"

float scalbnf(float x, int n)
{
    // This should be FLX_RADIX based but, the value is 2 on all machines we know of except the IBM 360 and derivatives.
    // So we just call ldexpf.
    return ldexpf(x, n);
}