#include "../math.h"

float nexttowardf(float x, long double y)
{
    return nextafterf(x, (float)y);
}