#include "../fenv.h"

int fegetround(void)
{
    fcontrol_t control_word;
    control_word = __FPU_read_control_word();

    return control_word.rounding_control;
}