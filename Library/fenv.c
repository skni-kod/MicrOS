#include "fenv.h"

// Additional content

fcontrol_t _FPU_read_control_word()
{
    fcontrol_t control_word;

    __asm__ (
        "fstcw %0 \n"
        : "=m"(control_word));
    return control_word;

}

void _FPU_write_control_word(fcontrol_t control_word)
{
__asm__ (
        "fclex \n" \
        "fldcw %0"
        :: "m"(control_word));
}

fexcept_t _FPU_read_status_word()
{
    fexcept_t status_word;

    __asm__ (
        "fstsw %0 \n"
        : "=m"(status_word));
    return status_word;
}

void _FPU_clear_exceptions()
{
    __asm__ (
        "fclex"
        ::);
}