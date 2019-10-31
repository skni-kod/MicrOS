#include "../math.h"

float tanhf(float x)
{
    return sinhf(x) / coshf(x);
}