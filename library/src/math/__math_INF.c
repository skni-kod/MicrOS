#include "../math.h"

double __math_INF()
{
    double_byte f;
    f.bytes[sizeof(double) - 1] = 0x7F;
    f.bytes[sizeof(double) - 2] = 0xF0;
    for(int i = sizeof(double) - 3; i >= 0; --i)
    {
        f.bytes[i] = 0x00;
    }
    return f.f;
}