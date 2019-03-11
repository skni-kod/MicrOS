#include "../math.h"

long double __math_INFl()
{
    long_double_byte f;
    f.bytes[sizeof(long double) - 1] = 0x7F;
    f.bytes[sizeof(long double) - 2] = 0xFF;
    return f.f;
}