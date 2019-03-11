#include "../fenv.h"

int fetestexcept(int excepts)
{
    if(excepts == 0)
    {
        return 0;
    }
    // We only have 6 excepts to check to cut varaible.
    unsigned char excepts_to_check = (unsigned char)excepts;

    // Set first 2 digit as 0 to make sure we won't check anything in first 2 bits.
    excepts_to_check &= 0x3F;

    // Get current environment of FPU.
    fexcept_t fe = __FPU_read_status_word();

    unsigned char * p_fe = (unsigned char *)&fe;
    excepts_to_check &= p_fe[0];
    return excepts_to_check;
}