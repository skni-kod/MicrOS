#include "../fenv.h"

fexcept_t _FPU_read_status_word()
{
    fexcept_t status_word;

    __asm__ (
        "fstsw %0 \n"
        : "=m"(status_word));
    return status_word;
}