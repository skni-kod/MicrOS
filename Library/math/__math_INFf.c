#include "../math.h"

float __math_INFf()
{
    float_byte f;
    f.bytes[sizeof(float) - 1] = 0x7F;
    f.bytes[sizeof(float) - 2] = 0x80;
    return f.f;
}