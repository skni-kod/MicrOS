#include "../fenv.h"

int fesetround(int round)
{
    // User passed wrong direction
    if(round < 0 || round > 3)
    {
        return 1;
    }
    fcontrol_t control_word;
    control_word = __FPU_read_control_word();
    control_word.rounding_control = (unsigned char) round;
    __FPU_write_control_word(control_word);
    return 0;
}