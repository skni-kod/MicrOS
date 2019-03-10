#include "../math.h"

long long int llround(double x)
{
    // Change FPU control to nearest
    fcontrol_t previous, current;
    previous = _FPU_read_control_word();
    current = previous;
    current.rounding_control = _FPU_ROUNDING_NEAREST_EVEN;
    _FPU_write_control_word(current);
    long long int result;
    __asm__ (
        "fldl %1 \n" \
        "frndint \n" \
        "fistp %0"
        : "=m"(result): "m"(x));
    // Restore previous FPU control
    _FPU_write_control_word(previous);
    return result;
}