#include "../math.h"

int __math_fpclasifyf(float x)
{
    __asm__ (
        "fld %1 \n" \
        "fxam \n" \
        "fstp %0"
        : "=m"(x): "m"(x));

    fexcept_t status_word = __FPU_read_status_word();

    // Unsupported
    if(status_word.condition_code_0 == 0 && status_word.condition_code_2 == 0 && status_word.condition_code_3 == 0)
    {
        return FP_WTF;
    }
    // NaN
    if(status_word.condition_code_0 == 1 && status_word.condition_code_2 == 0 && status_word.condition_code_3 == 0)
    {
        return FP_NAN;
    }
    // Normal finite number
    if(status_word.condition_code_0 == 0 && status_word.condition_code_2 == 1 && status_word.condition_code_3 == 0)
    {
        return FP_NORMAL;
    }
    // Infinity
    if(status_word.condition_code_0 == 1 && status_word.condition_code_2 == 1 && status_word.condition_code_3 == 0)
    {
        return FP_INFINITE;
    }
    // Zero
    if(status_word.condition_code_0 == 0 && status_word.condition_code_2 == 0 && status_word.condition_code_3 == 1)
    {
        return FP_ZERO;
    }
    // Empty
    if(status_word.condition_code_0 == 1 && status_word.condition_code_2 == 0 && status_word.condition_code_3 == 1)
    {
        return FP_WTF;
    }
    // Denormal number
    if(status_word.condition_code_0 == 0 && status_word.condition_code_2 == 1 && status_word.condition_code_3 == 1)
    {
        return FP_SUBNORMAL;
    }
    // None of above
    return FP_WTF;
}