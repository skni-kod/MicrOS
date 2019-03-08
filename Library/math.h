#include<float.h>
#include "errno.h"

#ifndef MATH_H
#define MATH_H

//! The base of natural logarithms.
#define M_E         2.7182818284590452354
//! The logarithm to base 2 of M_E.
#define M_LOG2E     1.4426950408889634074
//! The logarithm to base 10 of M_E.
#define M_LOG10E    0.43429448190325182765
//! The natural logarithm of 2.
#define M_LN2       0.69314718055994530942
//! The natural logarithm of 10.
#define M_LN10      2.30258509299404568402
//! Pi, the ratio of a circle’s circumference to its diameter.
#define M_PI        3.14159265358979323846
//! Pi divided by two.
#define M_PI_2      1.57079632679489661923
//! Pi divided by four.
#define M_PI_4      0.78539816339744830962
//! The reciprocal of pi (1/pi).
#define M_1_PI      0.31830988618379067154
//! Two times the reciprocal of pi.
#define M_2_PI      0.63661977236758134308
//! Two times the reciprocal of the square root of pi.
#define M_2_SQRTPI  1.12837916709551257390
//! The square root of two.
#define M_SQRT2     1.41421356237309504880
//! The reciprocal of the square root of two (also the square root of 1/2).
#define M_SQRT1_2   0.70710678118654752440

//! Indicates that floating-point operations use the variable errno to report errors.
#define MATH_ERRNO 1
//! Indicates that floating-point exceptions are used.
#define MATH_ERREXCEPT 2
//! Expands to an expression of type int that is either equal to MATH_ERRNO, or equal to MATH_ERREXCEPT, or equal to their bitwise OR (MATH_ERRNO | MATH_ERREXCEPT).
int _math_errhandling = MATH_ERRNO;
//! Macro to access _math_errhandling.
/*! The macro constant math_errhandling expands to an expression of type int that is either equal to MATH_ERRNO, or equal to MATH_ERREXCEPT, or equal to their bitwise OR (MATH_ERRNO | MATH_ERREXCEPT). */
#define math_errhandling _math_errhandling

#if FLT_EVAL_METHOD == 0
    //! Floating-point type.
    /*!
        Alias of one of the fundamental floating-point types at least as wide as float.
    */
    typedef float float_t;
    //! Floating-point type.
    /*!
        Alias of one of the fundamental floating-point types at least as wide as double.
    */
    typedef double double_t;
#elif FLT_EVAL_METHOD == 1
    //! Floating-point type.
    /*!
        Alias of one of the fundamental floating-point types at least as wide as float.
    */
    typedef double float_t;
    //! Floating-point type.
    /*!
        Alias of one of the fundamental floating-point types at least as wide as double.
    */
    typedef double double_t;
#elif FLT_EVAL_METHOD == 2
    //! Floating-point type.
    /*!
        Alias of one of the fundamental floating-point types at least as wide as float.
    */
    typedef long double float_t;
    //! Floating-point type.
    /*!
        Alias of one of the fundamental floating-point types at least as wide as double.
    */
    typedef long double double_t;
#else
    //! Floating-point type.
    /*!
        Alias of one of the fundamental floating-point types at least as wide as float.
    */
    typedef float float_t;
    //! Floating-point type.
    /*!
        Alias of one of the fundamental floating-point types at least as wide as double.
    */
    typedef double double_t;
#endif

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

//! Calculate cosine.
/*! 
    The cos functions compute the cosine of x (measured in radians).
    \param x First value.
    \return The cos functions return cos x.
*/
double cos(double x);

//! Calculate cosine.
/*! 
    The cosf functions compute the cosine of x (measured in radians).
    \param x First value.
    \return The cosf functions return cos x.
*/
float cosf(float x);


//! Calculate sine.
/*! 
    The sin functions compute the sine of x (measured in radians).
    \param x First value.
    \return The sin functions return sin x.
*/
double sin(double x);

//! Calculate sine.
/*! 
    The sinf functions compute the sine of x (measured in radians).
    \param x First value.
    \return The sinf functions return sin x.
*/
float sinf(float x);

//! Calculate tangent.
/*! 
    The tan functions compute the tangent of x (measured in radians).
    \param x First value.
    \return The tan functions return tan x.
*/
double tan(double x);

//! Calculate tangent.
/*! 
    The tanf functions compute the tangent of x (measured in radians).
    \param x First value.
    \return The tanf functions return tan x.
*/
float tanf(float x);

//! Calculate arc cosine.
/*! 
    The acos functions compute the principal value of the arc cosine of x. A domain error occurs for arguments not in the interval [−1, +1].
    \param x First value.
    \return The acos functions return arccos x in the interval [0,π ] radians.
*/
double acos(double x);

//! Calculate arc cosine.
/*! 
    The acosf functions compute the principal value of the arc cosine of x. A domain error occurs for arguments not in the interval [−1, +1].
    \param x First value.
    \return The acosf functions return arccos x in the interval [0,π ] radians.
*/
float acosf(float x);

//! Calculate arc sine.
/*! 
    The asin functions compute the principal value of the arc sine of x. A domain error occurs for arguments not in the interval [−1, +1].
    \param x First value.
    \return The asin functions return arcsin x in the interval [−π /2, +π /2] radians.
*/
double asin(double x);

//! Calculate arc sine.
/*! 
    The asinf functions compute the principal value of the arc sine of x. A domain error occurs for arguments not in the interval [−1, +1].
    \param x First value.
    \return The asinf functions return arcsin x in the interval [−π /2, +π /2] radians.
*/
float asinf(float x);

//! Calculate arc tangent.
/*! 
    The atan functions compute the principal value of the arc tangent of x.
    \param x First value.
    \return The atan functions return arctan x in the interval [−π /2, +π /2] radians.
*/
double atan(double x);

//! Calculate arc tangent.
/*! 
    The atanf functions compute the principal value of the arc tangent of x.
    \param x First value.
    \return The atanf functions return arctan x in the interval [−π /2, +π /2] radians.
*/
float atanf(float x);

//! Calculate arc tangent.
/*! 
    The atan2 functions compute the value of the arc tangent of y/x, using the signs of both arguments to determine the quadrant of the return value. A domain error may occur if both arguments are zero.
    \param x First value.
    \param y Second value.
    \return The atan2 functions return arctan y/x in the interval [−π , +π ] radians.
*/
double atan2(double x, double y);

//! Calculate arc tangent.
/*! 
    The atan2f functions compute the value of the arc tangent of y/x, using the signs of both arguments to determine the quadrant of the return value. A domain error may occur if both arguments are zero.
    \param x First value.
    \param y Second value.
    \return The atan2f functions return arctan y/x in the interval [−π , +π ] radians.
*/
float atan2f(float x, float y);

// Hyperbolic functions

//! Calculate hyperbolic cosine.
/*! 
    The cosh functions compute the hyperbolic cosine of x. A range error occurs if the magnitude of x is too large.
    \param x First value.
    \return The cosh functions return cosh x.
*/
double cosh(double x);

//! Calculate hyperbolic cosine.
/*! 
    The coshf functions compute the hyperbolic cosine of x. A range error occurs if the magnitude of x is too large.
    \param x First value.
    \return The coshf functions return cosh x.
*/
float coshf(float x);

//! Calculate hyperbolic sine.
/*! 
    The sinh functions compute the hyperbolic sine of x. A range error occurs if the magnitude of x is too large.
    \param x First value.
    \return The sinh functions return sinh x.
*/
double sinh(double x);

//! Calculate hyperbolic sine.
/*! 
    The sinhf functions compute the hyperbolic sine of x. A range error occurs if the magnitude of x is too large.
    \param x First value.
    \return The sinhf functions return sinh x.
*/
float sinhf(float x);

//! Calculate hyperbolic tangent.
/*! 
    The tanh functions compute the hyperbolic tangent of x.
    \param x First value.
    \return The tanh functions return tanh x.
*/
double tanh(double x);

//! Calculate hyperbolic tangent.
/*! 
    The tanhf functions compute the hyperbolic tangent of x.
    \param x First value.
    \return The tanhf functions return tanh x.
*/
float tanhf(float x);

//! Calculate arc hyperbolic cosine.
/*! 
    The acosh functions compute the (nonnegative) arc hyperbolic cosine of x. A domain error occurs for arguments less than 1.
    \param x First value.
    \return The acosh functions return arcosh x in the interval [0, +∞].
*/
double acosh(double x);

//! Calculate arc hyperbolic cosine.
/*! 
    The acoshf functions compute the (nonnegative) arc hyperbolic cosine of x. A domain error occurs for arguments less than 1.
    \param x First value.
    \return The acoshf functions return arcosh x in the interval [0, +∞].
*/
float acoshf(float x);

//! Calculate arc hyperbolic sine.
/*! 
    The asinh functions compute the arc hyperbolic sine of x.
    \param x First value.
    \return The asinh functions return arsinh x.
*/
double asinh(double x);

//! Calculate arc hyperbolic sine.
/*! 
    The asinhf functions compute the arc hyperbolic sine of x.
    \param x First value.
    \return The asinhf functions return arsinh x.
*/
float asinhf(float x);

//! Calculate arc hyperbolic tangent.
/*! 
    The atanh functions compute the arc hyperbolic tangent of x. A domain error occurs for arguments not in the interval [−1, +1]. A pole error may occur if the argument equals −1 or +1.
    \param x First value.
    \return The atanh functions return artanh x.
*/
double atanh(double x);

//! Calculate arc hyperbolic tangent.
/*! 
    The atanhf functions compute the arc hyperbolic tangent of x. A domain error occurs for arguments not in the interval [−1, +1]. A pole error may occur if the argument equals −1 or +1.
    \param x First value.
    \return The atanhf functions return artanh x.
*/
float atanhf(float x);

// Exponential and logarithmic functions

//! Calculate exponential function.
/*! 
    The exp functions compute the base-e exponential of x. A range error occurs if the magnitude of x is too large.
    \param x First value.
    \return The exp functions return e^x.
*/
double exp(double x);

//! Calculate exponential function.
/*! 
    The expf functions compute the base-e exponential of x. A range error occurs if the magnitude of x is too large.
    \param x First value.
    \return The expf functions return e^x.
*/
float expf(float x);

//! Calculate natural logarithm.
/*! 
    The log functions compute the base-e (natural) logarithm of x. A domain error occurs if the argument is negative. A pole error may occur if the argument is zero.
    \param x First value.
    \return The log functions return log_e x.
*/
double log(double x);

//! Calculate natural logarithm.
/*! 
    The logf functions compute the base-e (natural) logarithm of x. A domain error occurs if the argument is negative. A pole error may occur if the argument is zero.
    \param x First value.
    \return The logf functions return log_e x.
*/
float logf(float x);

//! Calculate 10 based logarithm.
/*! 
    The log10 functions compute the base-10 (common) logarithm of x. A domain error occurs if the argument is negative. A pole error may occur if the argument is zero.
    \param x First value.
    \return The log10 functions return log_10 x.
*/
double log10(double x);

//! Calculate 10 based logarithm.
/*! 
    The log10f functions compute the base-10 (common) logarithm of x. A domain error occurs if the argument is negative. A pole error may occur if the argument is zero.
    \param x First value.
    \return The log10f functions return log_10 x.
*/
float log10f(float x);

//! Break into fractional and integral parts.
/*! 
    The modf functions break the argument value into integral and fractional parts, each of which has the same type and sign as the argument. They store the integral part (in floating-point format) in the object pointed to by iptr.
    \param x First value.
    \param iptr Return of integral part.
    \return The modf functions return the signed fractional part of value.
*/
double modf(double x, double* iptr);

//! Break into fractional and integral parts.
/*! 
    The modff functions break the argument value into integral and fractional parts, each of which has the same type and sign as the argument. They store the integral part (in floating-point format) in the object pointed to by iptr.
    \param x First value.
    \param iptr Return of integral part.
    \return The modff functions return the signed fractional part of value.
*/
float modff(float x, float* iptr);

//! Calculate exponential function 2 based.
/*! 
    The exp2 functions compute the base-2 exponential of x. A range error occurs if the magnitude of x is too large.
    \param x First value.
    \return The exp2 functions return 2^x.
*/
double exp2(double x);

//! Calculate exponential function 2 based.
/*! 
    The exp2f functions compute the base-2 exponential of x. A range error occurs if the magnitude of x is too large.
    \param x First value.
    \return The exp2f functions return 2^x.
*/
float exp2f(float x);

//! Calculate exponential function minus 1.
/*! 
    The expm1 functions compute the base-e exponential of the argument, minus 1. A range error occurs if x is too large.
    \param x First value.
    \return The expm1 functions return e^x − 1.
*/
double expm1(double x);

//! Calculate exponential function minus 1.
/*! 
    The expm1f functions compute the base-e exponential of the argument, minus 1. A range error occurs if x is too large.
    \param x First value.
    \return The expm1f functions return e^x − 1.
*/
float expm1f(float x);

//! Calculate natural logarithm of value + 1.
/*! 
    The log1p functions compute the base-e (natural) logarithm of 1 plus the argument. A domain error occurs if the argument is less than −1. A pole error may occur if the argument equals −1.
    \param x First value.
    \return The log1p functions return log_e(1 + x).
*/
double log1p(double x);

//! Calculate natural logarithm of value + 1.
/*! 
    The log1pf functions compute the base-e (natural) logarithm of 1 plus the argument. A domain error occurs if the argument is less than −1. A pole error may occur if the argument equals −1.
    \param x First value.
    \return The log1pf functions return log_e(1 + x).
*/
float log1pf(float x);

//! Calculate 2 based logarithm.
/*! 
    The log2 functions compute the base-2 logarithm of x. A domain error occurs if the argument is less than zero. A pole error may occur if the argument is zero.
    \param x First value.
    \return The log2 functions return log_2 x.
*/
double log2(double x);

//! Calculate 2 based logarithm.
/*! 
    The log2f functions compute the base-2 logarithm of x. A domain error occurs if the argument is less than zero. A pole error may occur if the argument is zero.
    \param x First value.
    \return The log2f functions return log_2 x.
*/
float log2f(float x);

//! Calculate logarithm using FLT_RADIX.
/*! 
    The logb functions extract the exponent of x, as a signed integer value in floating-point format. If x is subnormal it is treated as though it were normalized; thus, for positive finite x, 1 ≤ x × FLT_RADIX^(−log_b(x)) < FLT_RADIX. A domain error or pole error may occur if the argument is zero.
    \param x First value.
    \return The logb functions return the signed exponent of x.
*/
double logb(double x);

//! Calculate logarithm using FLT_RADIX.
/*! 
    The logbf functions extract the exponent of x, as a signed integer value in floating-point format. If x is subnormal it is treated as though it were normalized; thus, for positive finite x, 1 ≤ x × FLT_RADIX^(−log_b(x)) < FLT_RADIX. A domain error or pole error may occur if the argument is zero.
    \param x First value.
    \return The logbf functions return the signed exponent of x.
*/
float logbf(float x);

// Power functions 

//! Calculate power.
/*! 
    The pow functions compute x raised to the power y. A domain error occurs if x is finite and negative and y is finite and not an integer value. A range error may occur. A domain error may occur if x is zero and y is zero. A domain error or pole error may occur if x is zero and y is less than zero.
    \param base Base of power.
    \param exponent Exponent of power.
    \return The pow functions return x^y.
*/
double pow(double base, double exponent);

//! Calculate power.
/*! 
    The powf functions compute x raised to the power y. A domain error occurs if x is finite and negative and y is finite and not an integer value. A range error may occur. A domain error may occur if x is zero and y is zero. A domain error or pole error may occur if x is zero and y is less than zero.
    \param base Base of power.
    \param exponent Exponent of power.
    \return The powf functions return x^y.
*/
float powf(float base, float exponent);

//! Calculate square root.
/*! 
    The sqrt functions compute the nonnegative square root of x. A domain error occurs if the argument is less than zero.
    \param x First value.
    \return The sqrt functions return √x.
*/
double sqrt(double x);

//! Calculate square root.
/*! 
    The sqrtf functions compute the nonnegative square root of x. A domain error occurs if the argument is less than zero.
    \param x First value.
    \return The sqrtf functions return √x.
*/
float sqrtf(float x);

// Nearest integer floating-point operations

//! Rounding up.
/*! 
    The ceil functions compute the smallest integer value not less than x.
    \param x First value.
    \return The ceil functions return ⎡x⎤, expressed as a floating-point number.
*/
double ceil(double x);

//! Rounding up.
/*! 
    The ceilf functions compute the smallest integer value not less than x.
    \param x First value.
    \return The ceilf functions return ⎡x⎤, expressed as a floating-point number.
*/
float ceilf(float x);

//! Rounding down.
/*! 
    The floor functions compute the largest integer value not greater than x.
    \param x First value.
    \return The floor functions return ⎣x⎦, expressed as a floating-point number.
*/
double floor(double x);

//! Rounding down.
/*! 
    The floorf functions compute the largest integer value not greater than x.
    \param x First value.
    \return The floorf functions return ⎣x⎦, expressed as a floating-point number.
*/
float floorf(float x);

//! Compute remainder of division.
/*! 
    The fmod functions compute the floating-point remainder of x/y.
    \param numer Dividend.
    \param denom Divider.
    \return The fmod functions return the value x − ny, for some integer n such that, if y is nonzero, the result has the same sign as x and magnitude less than the magnitude of y. If y is zero, a domain error occurs and the fmod functions return zero.
*/
double fmod(double numer, double denom);

//! Compute remainder of division.
/*! 
    The fmodf functions compute the floating-point remainder of x/y.
    \param numer Dividend.
    \param denom Divider.
    \return The fmodf functions return the value x − ny, for some integer n such that, if y is nonzero, the result has the same sign as x and magnitude less than the magnitude of y. If y is zero, a domain error occurs and the fmodf functions return zero.
*/
float fmodf(float numer, float denom);

//! Rounding towards zero.
/*! 
    The trunc functions round their argument to the integer value, in floating format, nearest to but no larger in magnitude than the argument.
    \param x First value.
    \return The trunc functions return the truncated integer value.
*/
double trunc(double x);

//! Rounding towards zero.
/*! 
    The truncf functions round their argument to the integer value, in floating format, nearest to but no larger in magnitude than the argument.
    \param x First value.
    \return The truncf functions return the truncated integer value.
*/
float truncf(float x);

//! Rounding.
/*! 
    The round functions round their argument to the nearest integer value in floating-point format, rounding halfway cases away from zero, regardless of the current rounding direction.
    \param x First value.
    \return The round functions return the rounded integer value.
*/
double round(double x);

//! Rounding.
/*! 
    The roundf functions round their argument to the nearest integer value in floating-point format, rounding halfway cases away from zero, regardless of the current rounding direction.
    \param x First value.
    \return The roundf functions return the rounded integer value.
*/
float roundf(float x);

//! Rounding.
/*! 
    The lround and llround functions round their argument to the nearest integer value, rounding halfway cases away from zero, regardless of the current rounding direction. If the rounded value is outside the range of the return type, the numeric result is unspecified and a domain error or range error may occur.
    \param x First value.
    \return The lround and llround functions return the rounded integer value.
*/
long int lround(double x);

//! Rounding.
/*! 
    The lround and llround functions round their argument to the nearest integer value, rounding halfway cases away from zero, regardless of the current rounding direction. If the rounded value is outside the range of the return type, the numeric result is unspecified and a domain error or range error may occur.
    \param x First value.
    \return The lround and llround functions return the rounded integer value.
*/
long int lroundf(float x);

//! Rounding.
/*! 
    The lround and llround functions round their argument to the nearest integer value, rounding halfway cases away from zero, regardless of the current rounding direction. If the rounded value is outside the range of the return type, the numeric result is unspecified and a domain error or range error may occur.
    \param x First value.
    \return The lround and llround functions return the rounded integer value.
*/
long long int llround(double x);

//! Rounding.
/*! 
    The lround and llround functions round their argument to the nearest integer value, rounding halfway cases away from zero, regardless of the current rounding direction. If the rounded value is outside the range of the return type, the numeric result is unspecified and a domain error or range error may occur.
    \param x First value.
    \return The lround and llround functions return the rounded integer value.
*/
long long int llroundf(float x);

//! Rounding.
/*! 
    The rint functions round their argument to an integer value in floating-point format, using the current rounding direction.
    \param x First value.
    \return The rint functions return the rounded integer value.
*/
double rint(double x);

//! Rounding.
/*! 
    The rintf functions round their argument to an integer value in floating-point format, using the current rounding direction.
    \param x First value.
    \return The rintf functions return the rounded integer value.
*/
float rintf(float x);

//! Rounding.
/*! 
    The lrint function round their argument to the nearest integer value, rounding according to the current rounding direction. If the rounded value is outside the range of the return type, the numeric result is unspecified and a domain error or range error may occur.
    \param x First value.
    \return The lrint function return the rounded integer value.
*/
long int lrint(double x);

//! Rounding.
/*! 
    The lrintf function round their argument to the nearest integer value, rounding according to the current rounding direction. If the rounded value is outside the range of the return type, the numeric result is unspecified and a domain error or range error may occur.
    \param x First value.
    \return The lrintf function return the rounded integer value.
*/
long int lrintf(float x);

//! Rounding.
/*! 
    The llrint function round their argument to the nearest integer value, rounding according to the current rounding direction. If the rounded value is outside the range of the return type, the numeric result is unspecified and a domain error or range error may occur.
    \param x First value.
    \return The llrint function return the rounded integer value.
*/
long long int llrint(double x);

//! Rounding.
/*! 
    The llrintf function round their argument to the nearest integer value, rounding according to the current rounding direction. If the rounded value is outside the range of the return type, the numeric result is unspecified and a domain error or range error may occur.
    \param x First value.
    \return The llrintf function return the rounded integer value.
*/
long long int llrintf(float x);

//! Rounding.
/*! 
    The nearbyint functions round their argument to an integer value in floating-point format, using the current rounding direction and without raising the ‘‘inexact’’ floatingpoint exception.
    \param x First value.
    \return The nearbyint functions return the rounded integer value.
*/
double nearbyint(double x);

//! Rounding.
/*! 
    The nearbyintf functions round their argument to an integer value in floating-point format, using the current rounding direction and without raising the ‘‘inexact’’ floatingpoint exception.
    \param x First value.
    \return The nearbyintf functions return the rounded integer value.
*/
float nearbyintf(float x);

//! Compute reminder required by IEC 60559.
/*! 
    The remainder functions compute the remainder x REM y required by IEC 60559.
    \param numer Dividend.
    \param denom Divider.
    \return The remainder functions return x REM y. If y is zero, whether a domain error occurs or the functions return zero is implementation defined.
*/
double remainder(double x, double y);

//! Compute reminder required by IEC 60559.
/*! 
    The remainderf functions compute the remainder x REM y required by IEC 60559.
    \param numer Dividend.
    \param denom Divider.
    \return The remainder functions return x REM y. If y is zero, whether a domain error occurs or the functions return zero is implementation defined.
*/
float remainderf(float x, float y);

// Minimum, maximum, difference functions

//! Positive difference.
/*! 
    The fdim functions determine the positive difference between their arguments.
    \param x First value.
    \param Y Second value.
    \return The fdim functions return the positive difference value.
*/
double fdim(double x, double y);

//! Positive difference.
/*! 
    The fdimf functions determine the positive difference between their arguments.
    \param x First value.
    \param Y Second value.
    \return The fdimf functions return the positive difference value.
*/
float fdimf(float x, float y);

//! Higher value.
/*! 
    The fmax functions determine the maximum numeric value of their arguments.
    \param x First value.
    \param Y Second value.
    \return The fmax functions return the maximum numeric value of their arguments.
*/
double fmax(double x, double y);

//! Higher value.
/*! 
    The fmaxf functions determine the maximum numeric value of their arguments.
    \param x First value.
    \param Y Second value.
    \return The fmaxf functions return the maximum numeric value of their arguments.
*/
float fmaxf(float x, float y);

//! Lower value.
/*! 
    The fmin functions determine the minimum numeric value of their arguments.
    \param x First value.
    \param Y Second value.
    \return The fmin functions return the minimum numeric value of their arguments.
*/
double fmin(double x, double y);

//! Lower value.
/*! 
    The fminf functions determine the minimum numeric value of their arguments.
    \param x First value.
    \param Y Second value.
    \return The fminf functions return the minimum numeric value of their arguments.
*/
float fminf(float x, float y);

// Other functions

//! Calculate absolute value.
/*! 
    The fabs functions compute the absolute value of a floating-point number x.
    \param x First value.
    \return The fabs functions return | x |.
*/
double fabs(double x);

//! Calculate absolute value.
/*! 
    The fabsf functions compute the absolute value of a floating-point number x.
    \param x First value.
    \return The fabsf functions return | x |.
*/
float fabsf(float x);

//! Multiply-add.
/*! 
    The fma functions compute (x × y) + z, rounded as one ternary operation: they compute the value (as if) to infinite precision and round once to the result format, according to the current rounding mode. A range error may occur.
    \param x First value.
    \param y Second value.
    \param z Third value.
    \return The fma functions return (x × y) + z, rounded as one ternary operation.
*/
double fma(double x, double y, double z);

//! Multiply-add.
/*! 
    The fmaf functions compute (x × y) + z, rounded as one ternary operation: they compute the value (as if) to infinite precision and round once to the result format, according to the current rounding mode. A range error may occur.
    \param x First value.
    \param y Second value.
    \param z Third value.
    \return The fmaf functions return (x × y) + z, rounded as one ternary operation.
*/
float fmaf(float x, float y, float z);

#endif