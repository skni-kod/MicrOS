#include "../fenv.h"

fenv_t _FPU_read_environment()
{
    fenv_t environment;

    __asm__ (
        "fstenv %0 \n"
        : "=m"(environment));
    return environment;
}