#include<float.h>

#ifndef MATH_H
#define MATH_H

#define M_E         2.7182818284590452354
#define M_LOG2E     1.4426950408889634074
#define M_LOG10E    0.43429448190325182765
#define M_LN2       0.69314718055994530942
#define M_LN10      2.30258509299404568402
#define M_PI        3.14159265358979323846
#define M_PI_2      1.57079632679489661923
#define M_PI_4      0.78539816339744830962
#define M_1_PI      0.31830988618379067154
#define M_2_PI      0.63661977236758134308
#define M_2_SQRTPI  1.12837916709551257390
#define M_SQRT2     1.41421356237309504880
#define M_SQRT1_2   0.70710678118654752440

// Comparison macros

//! The isnan macro determines whether its argument value is a NaN.
/*!
    \param x Value to check.
    \return Returns a nonzero value if and only if its argument has a NaN value.
*/
#define isnan(x) ((x) != (x))

//! The isnan macro determines whether its argument value is a NaN.
/*!
    The isgreater macro determines whether its first argument is greater than its second argument. The value of isgreater(x, y) is always equal to (x) > (y); however, unlike (x) > (y), isgreater(x, y) does not raise the ‘‘invalid’’ floating-point exception when x and y are unordered.
    \param x First value.
    \param y Second value.
    \return The isgreater macro returns the value of (x) > (y).
*/
#define isgreater(x,y) ((isnan(x) || isnan(y)) ? 0 : ( (x) > (y) ? 1 : 0 ))

//! Check if value x is greather than y.
/*!
    The isgreaterequal macro determines whether its first argument is greater than or equal to its second argument. The value of isgreaterequal(x, y) is always equal to (x) >= (y); howev er, unlike (x) >= (y), isgreaterequal(x, y) does not raise the ‘‘invalid’’ floating-point exception when x and y are unordered.
    \param x First value.
    \param y Second value.
    \return The isgreaterequal macro returns the value of (x) >= (y).
*/
#define isgreaterequal(x,y) ((isnan(x) || isnan(y)) ? 0 : ( (x) >= (y) ? 1 : 0 ))

//! Check if value x is less than y.
/*!
    The isless macro determines whether its first argument is less than its second argument. The value of isless(x, y) is always equal to (x) < (y); however, unlike (x) < (y), isless(x, y) does not raise the ‘‘invalid’’ floating-point exception when x and y are unordered.
    \param x First value.
    \param y Second value.
    \return The isless macro returns the value of (x) < (y).
*/
#define isless(x,y) ((isnan(x) || isnan(y)) ? 0 : ( (x) < (y) ? 1 : 0 ))

//! Check if value x is less or equal than y.
/*!
    The islessequal macro determines whether its first argument is less than or equal to its second argument. The value of islessequal(x, y) is always equal to(x) <= (y); however, unlike (x) <= (y), islessequal(x, y) does not raise the ‘‘invalid’’ floating-point exception when x and y are unordered.
    \param x First value.
    \param y Second value.
    \return The islessequal macro returns the value of (x) <= (y).
*/
#define islessequal(x,y) ((isnan(x) || isnan(y)) ? 0 : ( (x) <= (y) ? 1 : 0 ))

//! Check if value x is less or greather than y.
/*! 
    The islessgreater macro determines whether its first argument is less than or greater than its second argument. The islessgreater(x, y) macro is similar to (x) < (y) || (x) > (y); however, islessgreater(x, y) does not raise the ‘‘invalid’’ floating-point exception when x and y are unordered (nor does it evaluate x and y twice).
    \param x First value.
    \param y Second value.
    \return The islessgreater macro returns the value of (x) < (y) || (x) > (y).
*/
#define islessgreater(x,y) ((isnan(x) || isnan(y)) ? 0 : ( (x) < (y) || (x) > (y) ? 1 : 0 ))

//! Check if value x or y is unordered.
/*! 
    The isunordered macro determines whether its arguments are unordered.
    \param x First value.
    \param y Second value.
    \return The isunordered macro returns 1 if its arguments are unordered and 0 otherwise.
*/
#define isunordered(x,y) ((isnan(x) || isnan(y)) ? 1 : 0 ))

// Trigonometric functions

double cos(double x);
float cosf(float x);

double sin(double x);
float sinf(float x);

double tan(double x);
float tanf(float x);

double acos(double x);
float acosf(float x);

double asin(double x);
float asinf(float x);

double atan(double x);
float atanf(float x);

double atan2(double x, double y);
float atan2f(float x, float y);

// Hyperbolic functions

double cosh(double x);
float coshf(float x);

double sinh(double x);
float sinhf(float x);

double tanh(double x);
float tanhf(float x);

double acosh(double x);
float acoshf(float x);

double asinh(double x);
float asinhf(float x);

double atanh(double x);
float atanhf(float x);

// Exponential and logarithmic functions

double exp(double x);
float expf(float x);

double log(double x);
float logf(float x);

double log10(double x);
float log10f(float x);

double modf(double x, double* intpart);
float modff(float x, float* intpart);

double exp2(double x);
float exp2f(float x);

double expm1(double x);
float expm1f(float x);

double log1p(double x);
float log1pf(float x);

double log2(double x);
float log2f(float x);

double logb(double x);
float logbf(float x);

// Power functions 

double pow(double base, double exponent);
float powf(float base, float exponent);

double sqrt(double x);
float sqrtf(float x);

// Nearest integer floating-point operations

double ceil(double x);
float ceilf(float x);

double floor(double x);
float floorf(float x);

double trunc(double x);
float truncf(float x);

double round(double x);
float roundf(float x);

// Other functions

double fabs(double x);
float fabsf(float x);

#endif