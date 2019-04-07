#include "../fenv.h"

int fesetexceptflag(const fexcept_t *flagp, int excepts)
{
    if(flagp == NULL)
    {
        return 1;
    }
    if(excepts == 0)
    {
        return 0;
    }
    // We only have 6 excepts to set.
    unsigned char excepts_to_set = (unsigned char)excepts;
    excepts_to_set &= 0x3F;

    // Get exceptions flags
    fenv_t env;
    env = __FPU_read_environment();

    // Set only flags user asked for.

    if(excepts_to_set & 0x01)
    {
        env.status_word.invalid_operation = flagp->invalid_operation;
    }
    if(excepts_to_set & 0x02)
    {
        env.status_word.denormalized_operand = flagp->denormalized_operand;
    }
    if(excepts_to_set & 0x04)
    {
        env.status_word.zero_divide = flagp->zero_divide;
    }
    if(excepts_to_set & 0x08)
    {
        env.status_word.overflow = flagp->overflow;
    }
    if(excepts_to_set & 0x10)
    {
        env.status_word.underflow = flagp->underflow;
    }
    if(excepts_to_set & 0x20)
    {
        env.status_word.precision = flagp->precision;
    }
    __FPU_write_environment(env);
    return 0;
}