#include "../fenv.h"

int feraiseexcept(int excepts)
{
    if(excepts == 0)
    {
        return 0;
    }
    // We only have 6 excepts to raise.
    unsigned char excepts_to_raise = (unsigned char)excepts;
    excepts_to_raise &= 0x3F;

    // Get current environment of FPU.
    fenv_t env;
    env = __FPU_read_environment();

    // Set exception flags.
    fexcept_t * fe = &env.status_word;
    ((unsigned char *)fe)[0] |= excepts_to_raise;
    __FPU_write_environment(env);
    return 0;
}