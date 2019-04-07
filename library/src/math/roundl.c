#include "../math.h"

long double roundl(long double x)
{
    // Change FPU control to nearest
    fcontrol_t previous, current;
    previous = __FPU_read_control_word();
    current = previous;
    current.rounding_control = __FPU_ROUNDING_NEAREST_EVEN;
    __FPU_write_control_word(current);
    __asm__ (
        "fldt %1 \n" \
        "frndint \n" \
        "fstpt %0"
        : "=m"(x): "m"(x));
    // Restore previous FPU control
    __FPU_write_control_word(previous);
    return x;
}