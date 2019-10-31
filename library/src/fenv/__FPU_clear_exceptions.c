#include "../fenv.h"

void __FPU_clear_exceptions()
{
    __asm__ (
        "fclex"
        ::);
}