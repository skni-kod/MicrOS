#include "../math.h"

long double log1pl(long double x)
{
    return log2l(1 + x);
}