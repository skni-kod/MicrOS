#include "../math.h"

long double erfcl(long double x)
{
    return 1 - erfl(x);
}