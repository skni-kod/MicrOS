#include "../math.h"

double trunc(double x)
{
    // Change FPU control to truncate
    fcontrol_t previous, current;
    previous = _FPU_read_control_word();
    current = previous;
    current.rounding_control = _FPU_ROUNDING_TRUNCATE;
    _FPU_write_control_word(current);
    __asm__ (
        "fldl %1 \n" \
        "frndint \n" \
        "fstpl %0"
        : "=m"(x): "m"(x));

    // Restore previous FPU control
    _FPU_write_control_word(previous);
    return x;
}