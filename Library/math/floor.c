#include "../math.h"

double floor(double x)
{
    // Change FPU control to floor round
    fcontrol_t previous, current;
    previous = __FPU_read_control_word();
    current = previous;
    current.rounding_control = __FPU_ROUNDING_FLOOR;
    __FPU_write_control_word(current);
    __asm__ (
        "fldl %1 \n" \
        "frndint \n" \
        "fstpl %0"
        : "=m"(x): "m"(x));

    // Restore previous FPU control
    __FPU_write_control_word(previous);
    return x;
}