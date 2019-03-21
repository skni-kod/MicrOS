#include "../math.h"

float erfcf(float x)
{
    return 1 - erff(x);
}