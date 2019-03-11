#include "../math.h"

long double __math_INFl()
{
    long_double_byte f;
    f.bytes[sizeof(long double) - 3] = 0x7F;
    f.bytes[sizeof(long double) - 4] = 0xFF;
    return f.f;
}