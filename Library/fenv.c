#include "fenv.h"

void feclearexcept(int excepts)
{
    // We only have 6 excepts to clear to cut varaible.
    unsigned char excepts_to_clear = (char)excepts;

    // Get current environment of FPU.
    fenv_t env;
    env = _FPU_read_enviroment();

    // Set first 2 digit as 0 to make sure we won't overwrite anything in first 2 bits.
    excepts_to_clear &= 0x3F;

    // Negate value.
    excepts_to_clear = ~excepts_to_clear;

    // And excepts_to_clear and current enviroment.
    fexcept_t * fe = &env.status_word;
    ((unsigned char *)fe)[0] &= excepts_to_clear;
    _FPU_write_control(env);
}

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

fenv_t _FPU_read_enviroment()
{
    fenv_t enviroment;

    __asm__ (
        "fstenv %0 \n"
        : "=m"(enviroment));
    return enviroment;
}

void _FPU_write_control(fenv_t enviroment)
{
    __asm__ (
        "fldenv %0"
        :: "m"(enviroment));
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