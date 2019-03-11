#include "../math.h"

double __math_INF()
{
    double_byte f;
    f.bytes[sizeof(double) - 1] = 0x7F;
    f.bytes[sizeof(double) - 2] = 0xF0;
    return f.f;
}