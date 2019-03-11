#include "../fenv.h"

int feclearexcept(int excepts)
{
    if(excepts == 0)
    {
        return 0;
    }
    // We only have 6 excepts to clear to cut varaible.
    unsigned char excepts_to_clear = (unsigned char)excepts;

    // Get current environment of FPU.
    fenv_t env;
    env = __FPU_read_environment();

    // Set first 2 digit as 0 to make sure we won't overwrite anything in first 2 bits.
    excepts_to_clear &= 0x3F;

    // Negate value.
    excepts_to_clear = ~excepts_to_clear;

    // And excepts_to_clear and current environment.
    fexcept_t * fe = &env.status_word;
    ((unsigned char *)fe)[0] &= excepts_to_clear;
    __FPU_write_environment(env);
    return 0;
}