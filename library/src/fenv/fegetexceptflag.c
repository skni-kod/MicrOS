#include "../fenv.h"

int fegetexceptflag(fexcept_t *flagp, int excepts)
{
    if(flagp == NULL)
    {
        return 1;
    }
    if(excepts == 0)
    {
        return 0;
    }
    // We only have 6 excepts to get to cut varaible.
    unsigned char excepts_to_get = (unsigned char)excepts;
    excepts_to_get &= 0x3F;

    // Get exceptions flags
    fexcept_t exceptions = __FPU_read_status_word();

    // Set only flags user asked for.

    if(excepts_to_get & 0x01)
    {
        flagp->invalid_operation = exceptions.invalid_operation;
    }
    if(excepts_to_get & 0x02)
    {
        flagp->denormalized_operand = exceptions.denormalized_operand;
    }
    if(excepts_to_get & 0x04)
    {
        flagp->zero_divide = exceptions.zero_divide;
    }
    if(excepts_to_get & 0x08)
    {
        flagp->overflow = exceptions.overflow;
    }
    if(excepts_to_get & 0x10)
    {
        flagp->underflow = exceptions.underflow;
    }
    if(excepts_to_get & 0x20)
    {
        flagp->precision = exceptions.precision;
    }
    return 0;
}