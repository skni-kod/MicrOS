#include "../math.h"

float roundf(float x)
{
    // Change FPU control to nearest
    fcontrol_t previous, current;
    previous = _FPU_read_control_word();
    current = previous;
    current.rounding_control = _FPU_ROUNDING_NEAREST_EVEN;
    _FPU_write_control_word(current);
    __asm__ (
        "fld %1 \n" \
        "frndint \n" \
        "fstp %0"
        : "=m"(x): "m"(x));
    // Restore previous FPU control
    _FPU_write_control_word(previous);
    return x;
}