#include "../fenv.h"

fcontrol_t __FPU_read_control_word()
{
    fcontrol_t control_word;

    __asm__ (
        "fstcw %0 \n"
        : "=m"(control_word));
    return control_word;

}