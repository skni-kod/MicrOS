#include "../math.h"

float coshf(float x)
{
    return (expf(x) + expf(-x)) / 2;
}