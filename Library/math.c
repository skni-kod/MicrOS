#include "math.h"

//Trigonometric functions

double cos(double x)
{
    __asm__ (
        "fldl  %1 \n" \
        "fcos \n" \
        "fstpl %0"
        : "=m"(x): "m"(x));
    return x;
}

float cosf(float x)
{
    __asm__ (
        "fld  %1 \n" \
        "fcos \n" \
        "fstp %0"
        : "=m"(x): "m"(x));
    return x;
}

double sin(double x)
{
    __asm__ (
        "fldl  %1 \n" \
        "fsin \n" \
        "fstpl %0"
        : "=m"(x): "m"(x));
    return x;
}

float sinf(float x)
{
    __asm__ (
        "fld  %1 \n" \
        " fsin \n" \
        "fstp %0"
        : "=m"(x): "m"(x));
    return x;
}

double tan(double x)
{
    __asm__ (
        "fldl  %1 \n" \
        "fptan \n" \
        "fstpl %0 \n" \
        "fstpl %0"
        : "=m"(x): "m"(x));
    return x;
}

float tanf(float x)
{
    __asm__ (
        "fld  %1 \n" \
        "fptan \n" \
        "fstp %0 \n" \
        "fstp %0"
        : "=m"(x): "m"(x));
    return x;
}

double acos(double x)
{
    double two = 2;
    double asinX = asin(x);
    __asm__ (
        "fldl %1 \n" \
        "fldpi \n" \
        "fdivp \n" \
        "fldl %2 \n" \
        "fsubrp \n" \
        "fstpl %0"
        : "=m"(x): "m"(two), "m"(asinX));
    return x;
}

float acosf(float x)
{
    float two = 2;
    float asinX = asinf(x);
    __asm__ (
        "fld %1 \n" \
        "fldpi \n" \
        "fdivp \n" \
        "fld %2 \n" \
        "fsubrp \n" \
        "fstp %0"
        : "=m"(x): "m"(two), "m"(asinX));
    return x;
}

double asin(double x)
{
    __asm__ (
        "fldl  %1 \n" \
        "fldl  %1 \n" \
        "fmulp \n" \
        "fld1 \n" \
        "fsubp \n" \
        "fsqrt \n" \
        "fldl  %1 \n" \
        "fdivp \n" \
        "fld1 \n" \
        "fpatan \n" \
        "fstpl %0"
        : "=m"(x): "m"(x));
    return x;
}

float asinf(float x)
{
    __asm__ (
        "fld  %1 \n" \
        "fld  %1 \n" \
        "fmulp \n" \
        "fld1 \n" \
        "fsubp \n" \
        "fsqrt \n" \
        "fld  %1 \n" \
        "fdivp \n" \
        "fld1 \n" \
        "fpatan \n" \
        "fstp %0"
        : "=m"(x): "m"(x));
    return x;
}

double atan(double x)
{
    __asm__ (
        "fldl  %1 \n" \
        "fld1 \n" \
        "fpatan \n" \
        "fstpl %0 \n"
        : "=m"(x): "m"(x));
    return x;
}

float atanf(float x)
{
    __asm__ (
        "fld  %1 \n" \
        "fld1 \n" \
        "fpatan \n" \
        "fstp %0 \n"
        : "=m"(x): "m"(x));
    return x;
}

double atan2(double x, double y)
{
    __asm__ (
        "fldl %1 \n" \
        "fldl %2 \n" \
        "fpatan \n" \
        "fstpl %0 \n"
        : "=m"(x): "m"(x), "m"(y));
    return x;
}

float atan2f(float x, float y)
{
    __asm__ (
        "fld %1 \n" \
        "fld %2 \n" \
        "fpatan \n" \
        "fstp %0 \n"
        : "=m"(x): "m"(x), "m"(y));
    return x;
}

// Hyperbolic functions

double cosh(double x)
{
    return (exp(x) + exp(-x)) / 2;
}

float coshf(float x)
{
    return (expf(x) + expf(-x)) / 2;
}

double sinh(double x)
{
    return (exp(x) - exp(-x)) / 2;
}

float sinhf(float x)
{
    return (expf(x) - expf(-x)) / 2;
}

double tanh(double x)
{
    return sinh(x) / cosh(x);
}

float tanhf(float x)
{
    return sinhf(x) / coshf(x);
}

double acosh(double x)
{
    return log(x + sqrt(x * x - 1));
}

float acoshf(float x)
{
    return logf(x + sqrtf(x * x - 1));
}

double asinh(double x)
{
    return log(x + sqrt(x * x + 1));
}

float asinhf(float x)
{
    return logf(x + sqrtf(x * x + 1));
}

double atanh(double x)
{
    return 0.5 * log((1 + x)/(1 - x));
}

float atanhf(float x)
{
    return 0.5 * logf((1 + x)/(1 - x));
}

// Exponential and logarithmic functions

double exp(double x)
{
    return pow(M_E, x);
}

float expf(float x)
{
    return powf(M_E, x);
}

double log(double x)
{
    return log2(x)/log2(M_E);
}

float logf(float x)
{
    return log2f(x)/log2f(M_E);
}

double log10(double x)
{
    return log2(x)/log2(10.);
}

float log10f(float x)
{
    return log2f(x)/log2f(10.);
}

double modf(double x, double* intpart)
{
    double remainder;
    *intpart = (int)x;
    remainder = x - *intpart;
    return remainder;
}

float modff(float x, float* intpart)
{
    float remainder;
    *intpart = (int)x;
    remainder = x - *intpart;
    return remainder;
}

double exp2(double x)
{
    return pow(2, x);
}

float exp2f(float x)
{
    return powf(2, x);
}

double expm1(double x)
{
    return pow(M_E, x) - 1;
}

float expm1f(float x)
{
    return powf(M_E, x) - 1;
}

double log1p(double x)
{
    return log2(1 + x);
}

float log1pf(float x)
{
    return log2(1 + x);
}

double log2(double x)
{
    __asm__ (
        "fld1 \n" \
        "fldl %1 \n" \
        "fyl2x  \n" \
        "fstpl %0 \n"
        : "=m"(x): "m"(x));
    return x;
}

float log2f(float x)
{
    __asm__ (
        "fld1 \n" \
        "fld %1 \n" \
        "fyl2x \n" \
        "fstp %0 \n"
        : "=m"(x): "m"(x));
    return x;
}

// Power functions 

double pow(double base, double exponent)
{
    // Use 2^(y*log2(x)) 
    // Compute y*log2(x)
    double ex;
    __asm__ (
        "fldl %2 \n" \
        "fldl %1 \n" \
        "fyl2x \n" \
        "fstpl %0"
        : "=m"(ex): "m"(base), "m"(exponent));

    // Scale
    double integer, remainder;
    integer = (int)ex;
    remainder = ex - integer;

    // Compute 2^ex
    double resultBeforeScale;
    __asm__ (
        "fldl %1 \n" \
        "f2xm1 \n" \
        "fld1 \n" \
        "faddp \n" \
        "fstpl %0"
        : "=m"(resultBeforeScale): "m"(remainder));
    
    // Result after scale
    double result;
    __asm__ (
        "fldl %2 \n" \
        "fldl %1 \n" \
        "fscale \n" \
        "fstpl %0"
        : "=m"(result): "m"(resultBeforeScale), "m"(integer));
    return result;  
}

float powf(float base, float exponent)
{
    // Use 2^(y*log2(x)) 
    // Compute y*log2(x)
    float ex;
    __asm__ (
        "fld  %2 \n" \
        "fld  %1 \n" \
        "fyl2x \n" \
        "fstp %0"
        : "=m"(ex): "m"(base), "m"(exponent));

    // Scale
    float integer, remainder;
    integer = (int)ex;
    remainder = ex - integer;

    // Compute 2^ex
    float resultBeforeScale;
    __asm__ (
        "fld  %1 \n" \
        "f2xm1 \n" \
        "fld1 \n" \
        "faddp \n" \
        "fstp %0"
        : "=m"(resultBeforeScale): "m"(remainder));
    
    // Result after scale
    float result;
    __asm__ (
        "fld  %2 \n" \
        "fld  %1 \n" \
        "fscale \n" \
        "fstp %0"
        : "=m"(result): "m"(resultBeforeScale), "m"(integer));
    return result;  
}

double sqrt(double x)
{
    __asm__ (
        "fldl  %1 \n" \
        "fsqrt \n" \
        "fstpl %0"
        : "=m"(x): "m"(x));
    return x;  
}

float sqrtf(float x)
{
    __asm__ (
        "fld  %1 \n" \
        "fsqrt \n" \
        "fstp %0"
        : "=m"(x): "m"(x));
    return x;
}


//Nearest integer floating-point operations

double round(double x)
{
    __asm__ (
        "fld  %1 \n" \
        "frndint \n" \
        "fstpl %0"
        : "=m"(x): "m"(x));
    return x;
}

float roundf(float x)
{
    __asm__ (
        "fld  %1 \n" \
        "frndint \n" \
        "fstp %0"
        : "=m"(x): "m"(x));
    return x;
}

// Other functions

double fabs(double x)
{
    __asm__ (
        "fldl  %1 \n" \
        "fabs \n fstpl %0"
        : "=m"(x): "m"(x));
    return x;
}

float fabsf(float x)
{
    __asm__ (
        "fld  %1 \n" \
        "fabs \n fstp %0"
        : "=m"(x): "m"(x));
    return x;
}