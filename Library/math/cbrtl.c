#include "../math.h"

long double cbrtl(long double x)
{
    long double result = x;
    long double one_over_three = 1.0 / 3.0;
    int i;
    for(i = 0; i < 10; i++)
    {
        result = one_over_three * (x / pow(result, 2) + 2 * result);
    }
    return result;
}