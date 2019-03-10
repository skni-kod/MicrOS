#include "../math.h"

float atanhf(float x)
{
    return 0.5 * logf((1 + x)/(1 - x));
}