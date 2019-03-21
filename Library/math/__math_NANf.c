#include "../math.h"

float __math_NANf()
{
    float_byte f;
    f.bytes[sizeof(float) - 1] = 0x7F;
    f.bytes[sizeof(float) - 2] = 0xF8;
    return f.f;
}