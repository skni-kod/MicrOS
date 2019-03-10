#include "../fenv.h"

void _FPU_write_environment(fenv_t environment)
{
    __asm__ (
        "fldenv %0"
        :: "m"(environment));
}