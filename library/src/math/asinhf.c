#include "../math.h"

float asinhf(float x)
{
    return logf(x + sqrtf(x * x + 1));
}