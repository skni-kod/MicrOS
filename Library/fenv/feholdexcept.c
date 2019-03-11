#include "../fenv.h"

int feholdexcept(fenv_t *envp)
{
    if(envp == NULL)
    {
        return 1;
    }
    // Copy environment
    *envp = __FPU_read_environment();
    // Clear status
    feclearexcept(FE_ALL_EXCEPT);
    // Install a non-stop mode
    fcontrol_t control = __FPU_read_control_word();
    control.invalid_operation = 1;
    control.denormalized_operand = 1;
    control.zero_divide = 1; 
    control.overflow = 1;
    control.underflow = 1;
    control.precision = 1;
    __FPU_write_control_word(control);
    return 0;
}