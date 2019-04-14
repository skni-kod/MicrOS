#include "../math.h"

float __math_INFf()
{
    float_byte f;
    f.bytes[sizeof(float) - 1] = 0x7F;
    f.bytes[sizeof(float) - 2] = 0x80;
    for(int i = sizeof(float) - 3; i >= 0; --i)
    {
        f.bytes[i] = 0x00;
    }
    return f.f;
}