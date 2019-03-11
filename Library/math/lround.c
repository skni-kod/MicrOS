#include "../math.h"

long int lround(double x)
{
    // Change FPU control to nearest
    fcontrol_t previous, current;
    previous = __FPU_read_control_word();
    current = previous;
    current.rounding_control = __FPU_ROUNDING_NEAREST_EVEN;
    __FPU_write_control_word(current);
    long int result;
    __asm__ (
        "fldl %1 \n" \
        "frndint \n" \
        "fistp %0"
        : "=m"(result): "m"(x));
    // Restore previous FPU control
    __FPU_write_control_word(previous);
    return result;
}