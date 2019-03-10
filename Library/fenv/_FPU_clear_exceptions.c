#include "../fenv.h"

void _FPU_clear_exceptions()
{
    __asm__ (
        "fclex"
        ::);
}