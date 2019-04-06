#include <limits.h>
#include <float.h>
#include "errno.h"
#include "fenv.h"

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

//! Positive or negative infinity (overflow).
#define FP_INFINITE     0
//! Not-A-Number.
#define FP_NAN          1
//! Value of zero.
#define FP_ZERO         2
//! Sub-normal value (underflow)
#define FP_SUBNORMAL    3
//! Normal value (none of the above).
#define FP_NORMAL       4
//! In normal case this shouldn't be returned.
#define FP_WTF          5

//! Arg of ilogb is zero.
#define FP_ILOGB0       -2147483648
//! Arg of ilogb is NaN.
#define FP_ILOGBNAN     -2147483648

//! Indicates that floating-point operations use the variable errno to report errors.
#define MATH_ERRNO      1
//! Indicates that floating-point exceptions are used.
#define MATH_ERREXCEPT  2
//! Expands to an expression of type int that is either equal to MATH_ERRNO, or equal to MATH_ERREXCEPT, or equal to their bitwise OR (MATH_ERRNO | MATH_ERREXCEPT).
extern int _math_errhandling;
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

// Comparison macro

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

#ifdef __cplusplus
extern "C" {
#endif

// Trigonometric functions

//! Compute cosine.
/*! 
    The cos functions compute the cosine of x (measured in radians).
    \param x Value representing an angle expressed in radians.
    \return Cosine of x radians.
*/
double cos(double x);

//! Compute cosine.
/*! 
    The cosf functions compute the cosine of x (measured in radians).
    \param x Value representing an angle expressed in radians.
    \return Cosine of x radians.
*/
float cosf(float x);

//! Compute cosine.
/*! 
    The cosl functions compute the cosine of x (measured in radians).
    \param x Value representing an angle expressed in radians.
    \return Cosine of x radians.
*/
long double cosl(long double x);

//! Compute cosine.
/*! 
    The sin functions compute the sine of x (measured in radians).
    \param x Value representing an angle expressed in radians.
    \return Sine of x radians.
*/
double sin(double x);

//! Compute cosine.
/*! 
    The sinf functions compute the sine of x (measured in radians).
    \param x Value representing an angle expressed in radians.
    \return Sine of x radians.
*/
float sinf(float x);

//! Compute cosine.
/*! 
    The sinl functions compute the sine of x (measured in radians).
    \param x Value representing an angle expressed in radians.
    \return Sine of x radians.
*/
long double sinl(long double x);

//! Compute tangent.
/*! 
    The tan functions compute the tangent of x (measured in radians).
    \param x Value representing an angle, expressed in radians.
    \return Tangent of x radians.
*/
double tan(double x);

//! Compute tangent.
/*! 
    The tanf functions compute the tangent of x (measured in radians).
    \param x Value representing an angle, expressed in radians.
    \return Tangent of x radians.
*/
float tanf(float x);

//! Compute tangent.
/*! 
    The tanl functions compute the tangent of x (measured in radians).
    \param x Value representing an angle, expressed in radians.
    \return Tangent of x radians.
*/
long double tanl(long double x);

//! Compute arc cosine.
/*! 
    The acos functions compute the principal value of the arc cosine of x. A domain error occurs for arguments not in the interval [−1, +1].
    \param x Value whose arc cosine is computed, in the interval [-1,+1].
    \return The acos functions return arccos x in the interval [0,π ] radians.
*/
double acos(double x);

//! Compute arc cosine.
/*! 
    The acosf functions compute the principal value of the arc cosine of x. A domain error occurs for arguments not in the interval [−1, +1].
    \param x Value whose arc cosine is computed, in the interval [-1,+1].
    \return The acosf functions return arccos x in the interval [0,π ] radians.
*/
float acosf(float x);

//! Compute arc cosine.
/*! 
    The acosl functions compute the principal value of the arc cosine of x. A domain error occurs for arguments not in the interval [−1, +1].
    \param x Value whose arc cosine is computed, in the interval [-1,+1].
    \return The acosl functions return arccos x in the interval [0,π ] radians.
*/
long double acosl(long double x);

//! Compute arc sine.
/*! 
    The asin functions compute the principal value of the arc sine of x. A domain error occurs for arguments not in the interval [−1, +1].
    \param x Value whose arc cosine is computed, in the interval [-1,+1].
    \return The asin functions return arcsin x in the interval [−π /2, +π /2] radians.
*/
double asin(double x);

//! Compute arc sine.
/*! 
    The asinf functions compute the principal value of the arc sine of x. A domain error occurs for arguments not in the interval [−1, +1].
    \param x Value whose arc cosine is computed, in the interval [-1,+1].
    \return The asinf functions return arcsin x in the interval [−π /2, +π /2] radians.
*/
float asinf(float x);

//! Compute arc sine.
/*! 
    The asinl functions compute the principal value of the arc sine of x. A domain error occurs for arguments not in the interval [−1, +1].
    \param x Value whose arc cosine is computed, in the interval [-1,+1].
    \return The asinl functions return arcsin x in the interval [−π /2, +π /2] radians.
*/
long double asinl(long double x);

//! Compute arc tangent.
/*! 
    The atan functions compute the principal value of the arc tangent of x.
    \param x Value whose arc tangent is computed.
    \return The atan functions return arctan x in the interval [−π /2, +π /2] radians.
*/
double atan(double x);

//! Compute arc tangent.
/*! 
    The atanf functions compute the principal value of the arc tangent of x.
    \param x Value whose arc tangent is computed.
    \return The atanf functions return arctan x in the interval [−π /2, +π /2] radians.
*/
float atanf(float x);

//! Compute arc tangent.
/*! 
    The atanl functions compute the principal value of the arc tangent of x.
    \param x Value whose arc tangent is computed.
    \return The atanl functions return arctan x in the interval [−π /2, +π /2] radians.
*/
long double atanl(long double x);

//! Compute arc tangent with two parameters.
/*! 
    The atan2 functions compute the value of the arc tangent of y/x, using the signs of both arguments to determine the quadrant of the return value. A domain error may occur if both arguments are zero.
    \param x Value representing the proportion of the y-coordinate.
    \param y Value representing the proportion of the x-coordinate.
    \return The atan2 functions return arctan y/x in the interval [−π , +π ] radians.
*/
double atan2(double x, double y);

//! Compute arc tangent with two parameters.
/*! 
    The atan2f functions compute the value of the arc tangent of y/x, using the signs of both arguments to determine the quadrant of the return value. A domain error may occur if both arguments are zero.
    \param x Value representing the proportion of the y-coordinate.
    \param y Value representing the proportion of the x-coordinate.
    \return The atan2f functions return arctan y/x in the interval [−π , +π ] radians.
*/
float atan2f(float x, float y);

//! Compute arc tangent with two parameters.
/*! 
    The atan2l functions compute the value of the arc tangent of y/x, using the signs of both arguments to determine the quadrant of the return value. A domain error may occur if both arguments are zero.
    \param x Value representing the proportion of the y-coordinate.
    \param y Value representing the proportion of the x-coordinate.
    \return The atan2l functions return arctan y/x in the interval [−π , +π ] radians.
*/
long double atan2l(long double x, long double y);

// Hyperbolic functions

//! Compute hyperbolic cosine.
/*! 
    The cosh functions compute the hyperbolic cosine of x. A range error occurs if the magnitude of x is too large.
    \param x Value representing a hyperbolic angle.
    \return The cosh functions return cosh x.
*/
double cosh(double x);

//! Compute hyperbolic cosine.
/*! 
    The coshf functions compute the hyperbolic cosine of x. A range error occurs if the magnitude of x is too large.
    \param x Value representing a hyperbolic angle.
    \return The coshf functions return cosh x.
*/
float coshf(float x);

//! Compute hyperbolic cosine.
/*! 
    The coshl functions compute the hyperbolic cosine of x. A range error occurs if the magnitude of x is too large.
    \param x Value representing a hyperbolic angle.
    \return The coshl functions return cosh x.
*/
long double coshl(long double x);

//! Compute hyperbolic sine.
/*! 
    The sinh functions compute the hyperbolic sine of x. A range error occurs if the magnitude of x is too large.
    \param x Value representing a hyperbolic angle.
    \return The sinh functions return sinh x.
*/
double sinh(double x);

//! Compute hyperbolic sine.
/*! 
    The sinhf functions compute the hyperbolic sine of x. A range error occurs if the magnitude of x is too large.
    \param x Value representing a hyperbolic angle.
    \return The sinhf functions return sinh x.
*/
float sinhf(float x);

//! Compute hyperbolic sine.
/*! 
    The sinhl functions compute the hyperbolic sine of x. A range error occurs if the magnitude of x is too large.
    \param x Value representing a hyperbolic angle.
    \return The sinhl functions return sinh x.
*/
long double sinhl(long double x);

//! Compute hyperbolic tangent.
/*! 
    The tanh functions compute the hyperbolic tangent of x.
    \param x Value representing a hyperbolic angle.
    \return The tanh functions return tanh x.
*/
double tanh(double x);

//! Compute hyperbolic tangent.
/*! 
    The tanhf functions compute the hyperbolic tangent of x.
    \param x Value representing a hyperbolic angle.
    \return The tanhf functions return tanh x.
*/
float tanhf(float x);

//! Compute hyperbolic tangent.
/*! 
    The tanhl functions compute the hyperbolic tangent of x.
    \param x Value representing a hyperbolic angle.
    \return The tanhl functions return tanh x.
*/
long double tanhl(long double x);

//! Compute area hyperbolic cosine.
/*! 
    The acosh functions compute the (nonnegative) arc hyperbolic cosine of x. A domain error occurs for arguments less than 1.
    \param x Value whose area hyperbolic cosine is computed.
    \return The acosh functions return arcosh x in the interval [0, +∞].
*/
double acosh(double x);

//! Compute area hyperbolic cosine.
/*! 
    The acoshf functions compute the (nonnegative) arc hyperbolic cosine of x. A domain error occurs for arguments less than 1.
    \param x Value whose area hyperbolic cosine is computed.
    \return The acoshf functions return arcosh x in the interval [0, +∞].
*/
float acoshf(float x);

//! Compute area hyperbolic cosine.
/*! 
    The acoshl functions compute the (nonnegative) arc hyperbolic cosine of x. A domain error occurs for arguments less than 1.
    \param x Value whose area hyperbolic cosine is computed.
    \return The acoshl functions return arcosh x in the interval [0, +∞].
*/
long double acoshl(long double x);

//! Compute area hyperbolic sine.
/*! 
    The asinh functions compute the arc hyperbolic sine of x.
    \param x Value whose area hyperbolic sine is computed.
    \return The asinh functions return arsinh x.
*/
double asinh(double x);

//! Compute area hyperbolic sine.
/*! 
    The asinhf functions compute the arc hyperbolic sine of x.
    \param x Value whose area hyperbolic sine is computed.
    \return The asinhf functions return arsinh x.
*/
float asinhf(float x);

//! Compute area hyperbolic sine.
/*! 
    The asinhl functions compute the arc hyperbolic sine of x.
    \param x Value whose area hyperbolic sine is computed.
    \return The asinhl functions return arsinh x.
*/
long double asinhl(long double x);

//! Compute area hyperbolic tangent.
/*! 
    The atanh functions compute the arc hyperbolic tangent of x. A domain error occurs for arguments not in the interval [−1, +1]. A pole error may occur if the argument equals −1 or +1.
    \param x Value whose area hyperbolic tangent is computed, in the interval [-1,+1].
    \return The atanh functions return artanh x.
*/
double atanh(double x);

//! Compute area hyperbolic tangent.
/*! 
    The atanhf functions compute the arc hyperbolic tangent of x. A domain error occurs for arguments not in the interval [−1, +1]. A pole error may occur if the argument equals −1 or +1.
    \param x Value whose area hyperbolic tangent is computed, in the interval [-1,+1].
    \return The atanhf functions return artanh x.
*/
float atanhf(float x);

//! Compute area hyperbolic tangent.
/*! 
    The atanhl functions compute the arc hyperbolic tangent of x. A domain error occurs for arguments not in the interval [−1, +1]. A pole error may occur if the argument equals −1 or +1.
    \param x Value whose area hyperbolic tangent is computed, in the interval [-1,+1].
    \return The atanhl functions return artanh x.
*/
long double atanhl(long double x);

// Exponential and logarithmic functions

//! Compute exponential function.
/*! 
    The exp functions compute the base-e exponential of x. A range error occurs if the magnitude of x is too large.
    \param x Value of the exponent.
    \return The exp functions return e^x.
*/
double exp(double x);

//! Compute exponential function.
/*! 
    The expf functions compute the base-e exponential of x. A range error occurs if the magnitude of x is too large.
    \param x Value of the exponent.
    \return The expf functions return e^x.
*/
float expf(float x);

//! Compute exponential function.
/*! 
    The expl functions compute the base-e exponential of x. A range error occurs if the magnitude of x is too large.
    \param x Value of the exponent.
    \return The expl functions return e^x.
*/
long double expl(long double x);

//! Get significand and exponent.
/*! 
    The frexp functions break a floating-point number into a normalized fraction and an integral power of 2. They store the integer in the int object pointed to by exp.
    \param x Value to be decomposed.
    \param exp Pointer to an int where the value of the exponent is stored.
    \return If value is not a floating-point number or if the integral power of 2 is outside the range of int, the results are unspecified. Otherwise, the frexp functions return the value x, such that x has a magnitude in the interval [1/2, 1) or zero, and value equals x × 2^exp. If value is zero, both parts of the result are zero.
*/
double frexp(double x, int* exp);

//! Get significand and exponent.
/*! 
    The frexpf functions break a floating-point number into a normalized fraction and an integral power of 2. They store the integer in the int object pointed to by exp.
    \param x Value to be decomposed.
    \param exp Pointer to an int where the value of the exponent is stored.
    \return If value is not a floating-point number or if the integral power of 2 is outside the range of int, the results are unspecified. Otherwise, the frexpf functions return the value x, such that x has a magnitude in the interval [1/2, 1) or zero, and value equals x × 2^exp. If value is zero, both parts of the result are zero.
*/
float frexpf(float x, int* exp);

//! Get significand and exponent.
/*! 
    The frexpl functions break a floating-point number into a normalized fraction and an integral power of 2. They store the integer in the int object pointed to by exp.
    \param x Value to be decomposed.
    \param exp Pointer to an int where the value of the exponent is stored.
    \return If value is not a floating-point number or if the integral power of 2 is outside the range of int, the results are unspecified. Otherwise, the frexp functions return the value x, such that x has a magnitude in the interval [1/2, 1) or zero, and value equals x × 2^exp. If value is zero, both parts of the result are zero.
*/
long double frexpl(long double x, int* exp);

//! Generate value from significand and exponent.
/*! 
    The ldexp functions multiply a floating-point number by an integral power of 2. A range error may occur.
    \param x Generate value from significand and exponent
    \param exp Value of the exponent.
    \return The ldexp functions return x * 2^exp.
*/
double ldexp(double x, int exp);

//! Generate value from significand and exponent.
/*! 
    The ldexpf functions multiply a floating-point number by an integral power of 2. A range error may occur.
    \param x Generate value from significand and exponent
    \param exp Value of the exponent.
    \return The ldexpf functions return x * 2^exp.
*/
float ldexpf(float x, int exp);

//! Generate value from significand and exponent.
/*! 
    The ldexpl functions multiply a floating-point number by an integral power of 2. A range error may occur.
    \param x Generate value from significand and exponent
    \param exp Value of the exponent.
    \return The ldexpl functions return x * 2^exp.
*/
long double ldexpl(long double x, int exp);

//! Compute natural logarithm.
/*! 
    The log functions compute the base-e (natural) logarithm of x. A domain error occurs if the argument is negative. A pole error may occur if the argument is zero.
    \param x Value whose logarithm is calculated.
    \return The log functions return log_e x.
*/
double log(double x);

//! Compute natural logarithm.
/*! 
    The logf functions compute the base-e (natural) logarithm of x. A domain error occurs if the argument is negative. A pole error may occur if the argument is zero.
    \param x Value whose logarithm is calculated.
    \return The logf functions return log_e x.
*/
float logf(float x);

//! Compute natural logarithm.
/*! 
    The logl functions compute the base-e (natural) logarithm of x. A domain error occurs if the argument is negative. A pole error may occur if the argument is zero.
    \param x Value whose logarithm is calculated.
    \return The logl functions return log_e x.
*/
long double logl(long double x);

//! Compute 10 based logarithm.
/*! 
    The log10 functions compute the base-10 (common) logarithm of x. A domain error occurs if the argument is negative. A pole error may occur if the argument is zero.
    \param x Value whose logarithm is calculated.
    \return The log10 functions return log_10 x.
*/
double log10(double x);

//! Compute 10 based logarithm.
/*! 
    The log10f functions compute the base-10 (common) logarithm of x. A domain error occurs if the argument is negative. A pole error may occur if the argument is zero.
    \param x Value whose logarithm is calculated.
    \return The log10f functions return log_10 x.
*/
float log10f(float x);

//! Compute 10 based logarithm.
/*! 
    The log10l functions compute the base-10 (common) logarithm of x. A domain error occurs if the argument is negative. A pole error may occur if the argument is zero.
    \param x Value whose logarithm is calculated.
    \return The log10l functions return log_10 x.
*/
long double log10l(long double x);

//! Break into fractional and integral parts.
/*! 
    The modf functions break the argument value into integral and fractional parts, each of which has the same type and sign as the argument. They store the integral part (in floating-point format) in the object pointed to by iptr.
    \param x Floating point value to break into parts.
    \param iptr Pointer to an object (of the same type as x) where the integral part is stored with the same sign as x.
    \return The modf functions return the signed fractional part of value.
*/
double modf(double x, double* iptr);

//! Break into fractional and integral parts.
/*! 
    The modff functions break the argument value into integral and fractional parts, each of which has the same type and sign as the argument. They store the integral part (in floating-point format) in the object pointed to by iptr.
    \param x Floating point value to break into parts.
    \param iptr Pointer to an object (of the same type as x) where the integral part is stored with the same sign as x.
    \return The modff functions return the signed fractional part of value.
*/
float modff(float x, float* iptr);

//! Break into fractional and integral parts.
/*! 
    The modfl functions break the argument value into integral and fractional parts, each of which has the same type and sign as the argument. They store the integral part (in floating-point format) in the object pointed to by iptr.
    \param x Floating point value to break into parts.
    \param iptr Pointer to an object (of the same type as x) where the integral part is stored with the same sign as x.
    \return The modfl functions return the signed fractional part of value.
*/
long double modfl(long double x, long double* iptr);

//! Compute exponential function 2 based.
/*! 
    The exp2 functions compute the base-2 exponential of x. A range error occurs if the magnitude of x is too large.
    \param x Value of the exponent.
    \return The exp2 functions return 2^x.
*/
double exp2(double x);

//! Compute exponential function 2 based.
/*! 
    The exp2f functions compute the base-2 exponential of x. A range error occurs if the magnitude of x is too large.
    \param x Value of the exponent.
    \return The exp2f functions return 2^x.
*/
float exp2f(float x);

//! Compute exponential function 2 based.
/*! 
    The exp2l functions compute the base-2 exponential of x. A range error occurs if the magnitude of x is too large.
    \param x Value of the exponent.
    \return The exp2l functions return 2^x.
*/
long double exp2l(long double x);

//! Compute exponential function minus 1.
/*! 
    The expm1 functions compute the base-e exponential of the argument, minus 1. A range error occurs if x is too large.
    \param x Value of the exponent.
    \return The expm1 functions return e^x − 1.
*/
double expm1(double x);

//! Compute exponential function minus 1.
/*! 
    The expm1f functions compute the base-e exponential of the argument, minus 1. A range error occurs if x is too large.
    \param x Value of the exponent.
    \return The expm1f functions return e^x − 1.
*/
float expm1f(float x);

//! Compute exponential function minus 1.
/*! 
    The expm1l functions compute the base-e exponential of the argument, minus 1. A range error occurs if x is too large.
    \param x Value of the exponent.
    \return The expm1l functions return e^x − 1.
*/
long double expm1l(long double x);

//! Integer binary logarithm.
/*! 
    The ilogb functions extract the exponent of x as a signed int value. If x is zero they compute the value FP_ILOGB0; if x is infinite they compute the value INT_MAX; if x is a NaN they compute the value FP_ILOGBNAN; otherwise, they are equivalent to calling the corresponding logb function and casting the returned value to type int. A domain error or range error may occur if x is zero, infinite, or NaN. If the correct value is outside the range of the return type, the numeric result is unspecified.
    \param x Value whose ilogb is returned.
    \return The ilogb functions return the exponent of x as a signed int value.
*/
int ilogb(double x);

//! Integer binary logarithm.
/*! 
    The ilogbf functions extract the exponent of x as a signed int value. If x is zero they compute the value FP_ILOGB0; if x is infinite they compute the value INT_MAX; if x is a NaN they compute the value FP_ILOGBNAN; otherwise, they are equivalent to calling the corresponding logb function and casting the returned value to type int. A domain error or range error may occur if x is zero, infinite, or NaN. If the correct value is outside the range of the return type, the numeric result is unspecified.
    \param x Value whose ilogbf is returned.
    \return The ilogbf functions return the exponent of x as a signed int value.
*/
int ilogbf(float x);

//! Integer binary logarithm.
/*! 
    The ilogbl functions extract the exponent of x as a signed int value. If x is zero they compute the value FP_ILOGB0; if x is infinite they compute the value INT_MAX; if x is a NaN they compute the value FP_ILOGBNAN; otherwise, they are equivalent to calling the corresponding logb function and casting the returned value to type int. A domain error or range error may occur if x is zero, infinite, or NaN. If the correct value is outside the range of the return type, the numeric result is unspecified.
    \param x Value whose ilogbl is returned.
    \return The ilogbl functions return the exponent of x as a signed int value.
*/
int ilogbl(long double x);

//! Compute natural logarithm of value + 1.
/*! 
    The log1p functions compute the base-e (natural) logarithm of 1 plus the argument. A domain error occurs if the argument is less than −1. A pole error may occur if the argument equals −1.
    \param x Value whose logarithm is calculated.
    \return The log1p functions return log_e(1 + x).
*/
double log1p(double x);

//! Compute natural logarithm of value + 1.
/*! 
    The log1pf functions compute the base-e (natural) logarithm of 1 plus the argument. A domain error occurs if the argument is less than −1. A pole error may occur if the argument equals −1.
    \param x Value whose logarithm is calculated.
    \return The log1pf functions return log_e(1 + x).
*/
float log1pf(float x);

//! Compute natural logarithm of value + 1.
/*! 
    The log1pl functions compute the base-e (natural) logarithm of 1 plus the argument. A domain error occurs if the argument is less than −1. A pole error may occur if the argument equals −1.
    \param x Value whose logarithm is calculated.
    \return The log1pl functions return log_e(1 + x).
*/
long double log1pl(long double x);

//! Compute 2 based logarithm.
/*! 
    The log2 functions compute the base-2 logarithm of x. A domain error occurs if the argument is less than zero. A pole error may occur if the argument is zero.
    \param x Value whose logarithm is calculated.
    \return The log2 functions return log_2 x.
*/
double log2(double x);

//! Compute 2 based logarithm.
/*! 
    The log2f functions compute the base-2 logarithm of x. A domain error occurs if the argument is less than zero. A pole error may occur if the argument is zero.
    \param x Value whose logarithm is calculated.
    \return The log2f functions return log_2 x.
*/
float log2f(float x);

//! Compute 2 based logarithm.
/*! 
    The log2l functions compute the base-2 logarithm of x. A domain error occurs if the argument is less than zero. A pole error may occur if the argument is zero.
    \param x Value whose logarithm is calculated.
    \return The log2l functions return log_2 x.
*/
long double log2l(long double x);

//! Compute logarithm using FLT_RADIX.
/*! 
    The logb functions extract the exponent of x, as a signed integer value in floating-point format. If x is subnormal it is treated as though it were normalized; thus, for positive finite x, 1 ≤ x × FLT_RADIX^(−log_b(x)) < FLT_RADIX. A domain error or pole error may occur if the argument is zero.
    \param x Value whose logarithm is calculated.
    \return The logb functions return the signed exponent of x.
*/
double logb(double x);

//! Compute logarithm using FLT_RADIX.
/*! 
    The logbf functions extract the exponent of x, as a signed integer value in floating-point format. If x is subnormal it is treated as though it were normalized; thus, for positive finite x, 1 ≤ x × FLT_RADIX^(−log_b(x)) < FLT_RADIX. A domain error or pole error may occur if the argument is zero.
    \param x Value whose logarithm is calculated.
    \return The logbf functions return the signed exponent of x.
*/
float logbf(float x);

//! Compute logarithm using FLT_RADIX.
/*! 
    The logbl functions extract the exponent of x, as a signed integer value in floating-point format. If x is subnormal it is treated as though it were normalized; thus, for positive finite x, 1 ≤ x × FLT_RADIX^(−log_b(x)) < FLT_RADIX. A domain error or pole error may occur if the argument is zero.
    \param x Value whose logarithm is calculated.
    \return The logbl functions return the signed exponent of x.
*/
long double logbl(long double x);

//! Scale significand using floating-point base exponent.
/*! 
    The scalbn function compute x × FLT_RADIX^n efficiently, not normally by computing FLT_RADIX^n explicitly. A range error may occur.
    \param x Value representing the significand.
    \param n Value of the exponent.
    \return The scalbn function return x × FLT_RADIX^n.
*/
double scalbn(double x, int n);

//! Scale significand using floating-point base exponent.
/*! 
    The scalbnf function compute x × FLT_RADIX^n efficiently, not normally by computing FLT_RADIX^n explicitly. A range error may occur.
    \param x Value representing the significand.
    \param n Value of the exponent.
    \return The scalbnf function return x × FLT_RADIX^n.
*/
float scalbnf(float x, int n);

//! Scale significand using floating-point base exponent.
/*! 
    The scalbnl function compute x × FLT_RADIX^n efficiently, not normally by computing FLT_RADIX^n explicitly. A range error may occur.
    \param x Value representing the significand.
    \param n Value of the exponent.
    \return The scalbnl function return x × FLT_RADIX^n.
*/
long double scalbnl(long double x, int n);

//! Scale significand using floating-point base exponent.
/*! 
    The scalbln function compute x × FLT_RADIX^n efficiently, not normally by computing FLT_RADIX^n explicitly. A range error may occur.
    \param x Value representing the significand.
    \param n Value of the exponent.
    \return The scalbln function return x × FLT_RADIX^n.
*/
double scalbln(double x, long int n);

//! Scale significand using floating-point base exponent.
/*! 
    The scalblnf function compute x × FLT_RADIX^n efficiently, not normally by computing FLT_RADIX^n explicitly. A range error may occur.
    \param x Value representing the significand.
    \param n Value of the exponent.
    \return The scalblnf function return x × FLT_RADIX^n.
*/
float scalblnf(float x, long int n);

//! Scale significand using floating-point base exponent.
/*! 
    The scalblnl function compute x × FLT_RADIX^n efficiently, not normally by computing FLT_RADIX^n explicitly. A range error may occur.
    \param x Value representing the significand.
    \param n Value of the exponent.
    \return The scalblnl function return x × FLT_RADIX^n.
*/
long double scalblnl(long double x, long int n);

// Power functions 

//! Compute power.
/*! 
    The pow functions compute x raised to the power y. A domain error occurs if x is finite and negative and y is finite and not an integer value. A range error may occur. A domain error may occur if x is zero and y is zero. A domain error or pole error may occur if x is zero and y is less than zero.
    \param base Base of power.
    \param exponent Exponent of power.
    \return The pow functions return x^y.
*/
double pow(double base, double exponent);

//! Compute power.
/*! 
    The powf functions compute x raised to the power y. A domain error occurs if x is finite and negative and y is finite and not an integer value. A range error may occur. A domain error may occur if x is zero and y is zero. A domain error or pole error may occur if x is zero and y is less than zero.
    \param base Base of power.
    \param exponent Exponent of power.
    \return The powf functions return x^y.
*/
float powf(float base, float exponent);

//! Compute power.
/*! 
    The powl functions compute x raised to the power y. A domain error occurs if x is finite and negative and y is finite and not an integer value. A range error may occur. A domain error may occur if x is zero and y is zero. A domain error or pole error may occur if x is zero and y is less than zero.
    \param base Base of power.
    \param exponent Exponent of power.
    \return The powl functions return x^y.
*/
long double powl(long double base, long double exponent);

//! Compute square root.
/*! 
    The sqrt functions compute the nonnegative square root of x. A domain error occurs if the argument is less than zero.
    \param x Value whose square root is computed.
    \return The sqrt functions return √x.
*/
double sqrt(double x);

//! Compute square root.
/*! 
    The sqrtf functions compute the nonnegative square root of x. A domain error occurs if the argument is less than zero.
    \param x Value whose square root is computed.
    \return The sqrtf functions return √x.
*/
float sqrtf(float x);

//! Compute square root.
/*! 
    The sqrtl functions compute the nonnegative square root of x. A domain error occurs if the argument is less than zero.
    \param x Value whose square root is computed.
    \return The sqrtl functions return √x.
*/
long double sqrtl(long double x);

//! Compute cubic root.
/*! 
    The cbrt functions compute the cubic root of x.
    \param x Value whose cubic root is computed.
    \return The cbrt functions return 3√x.
*/
double cbrt(double x);

//! Compute cubic root.
/*! 
    The cbrtf functions compute the cubic root of x.
    \param x Value whose cubic root is computed.
    \return The cbrtf functions return 3√x.
*/
float cbrtf(float x);

//! Compute cubic root.
/*! 
    The cbrtl functions compute the cubic root of x.
    \param x Value whose cubic root is computed.
    \return The cbrtl functions return 3√x.
*/
long double cbrtl(long double x);

//! Compute hypotenuse.
/*! 
    The hypot functions compute the square root of the sum of the squares of x and y, without undue overflow or underflow. A range error may occur.
    \param x Floating point value corresponding to the leg of a right-angled triangle for which the hypotenuse is computed.
    \param y Floating point value corresponding to the leg of a right-angled triangle for which the hypotenuse is computed.
    \return The hypot functions return √(x^2+y^2).
*/
double hypot(double x, double y);

//! Compute hypotenuse.
/*! 
    The hypotf functions compute the square root of the sum of the squares of x and y, without undue overflow or underflow. A range error may occur.
    \param x Floating point value corresponding to the leg of a right-angled triangle for which the hypotenuse is computed.
    \param y Floating point value corresponding to the leg of a right-angled triangle for which the hypotenuse is computed.
    \return The hypotf functions return √(x^2+y^2).
*/
float hypotf(float x, float y);

//! Compute hypotenuse.
/*! 
    The hypotl functions compute the square root of the sum of the squares of x and y, without undue overflow or underflow. A range error may occur.
    \param x Floating point value corresponding to the leg of a right-angled triangle for which the hypotenuse is computed.
    \param y Floating point value corresponding to the leg of a right-angled triangle for which the hypotenuse is computed.
    \return The hypotl functions return √(x^2+y^2).
*/
long double hypotl(long double x, long double y);

// Error and gamma functions

//! Compute error function.
/*! 
    The erf functions compute the error function of x.
    \param x Parameter for the error function.
    \return Error function value for x.
*/
double erf(double x);

//! Compute error function.
/*! 
    The erff functions compute the error function of x.
    \param x Parameter for the error function.
    \return Error function value for x.
*/
float erff(float x);

//! Compute error function.
/*! 
    The erfl functions compute the error function of x.
    \param x Parameter for the error function.
    \return Error function value for x.
*/
long double erfl(long double x);

//! Compute complementary error function.
/*! 
    The erfc functions compute the complementary error function of x. A range error occurs if x is too large.
    \param x Parameter for the complementary error function.
    \return Complementary error function value for x.
*/
double erfc(double x);

//! Compute complementary error function.
/*! 
    The erfcf functions compute the complementary error function of x. A range error occurs if x is too large.
    \param x Parameter for the complementary error function.
    \return Complementary error function value for x.
*/
float erfcf(float x);

//! Compute complementary error function.
/*! 
    The erfcl functions compute the complementary error function of x. A range error occurs if x is too large.
    \param x Parameter for the complementary error function.
    \return Complementary error function value for x.
*/
long double erfcl(long double x);

//! Compute gamma function.
/*! 
    The tgamma functions compute the gamma function of x. A domain error or pole error may occur if x is a negative integer or zero. A range error occurs if the magnitude of x is too large and may occur if the magnitude of x is too small.
    \param x Parameter for the gamma function.
    \return The tgamma functions return Γ(x).
*/
double tgamma(double x);

//! Compute gamma function.
/*! 
    The tgammaf functions compute the gamma function of x. A domain error or pole error may occur if x is a negative integer or zero. A range error occurs if the magnitude of x is too large and may occur if the magnitude of x is too small.
    \param x Parameter for the gamma function.
    \return The tgamma functions return Γ(x).
*/
float tgammaf(float x);

//! Compute gamma function.
/*! 
    The tgammal functions compute the gamma function of x. A domain error or pole error may occur if x is a negative integer or zero. A range error occurs if the magnitude of x is too large and may occur if the magnitude of x is too small.
    \param x Parameter for the gamma function.
    \return The tgamma functions return Γ(x).
*/
long double tgammal(long double x);

//! Compute gamma function.
/*! 
    The lgamma functions compute the natural logarithm of the absolute value of gamma of x. A range error occurs if x is too large. A pole error may occur if x is a negative integer or zero.
    \param x Parameter for the log-gamma function.
    \return The lgamma functions return log_e | Γ(x) |.
*/
double lgamma(double x);

//! Compute gamma function.
/*! 
    The lgammaf functions compute the natural logarithm of the absolute value of gamma of x. A range error occurs if x is too large. A pole error may occur if x is a negative integer or zero.
    \param x Parameter for the log-gamma function.
    \return The lgammaf functions return log_e | Γ(x) |.
*/
float lgammaf(float x);

//! Compute gamma function.
/*! 
    The lgammal functions compute the natural logarithm of the absolute value of gamma of x. A range error occurs if x is too large. A pole error may occur if x is a negative integer or zero.
    \param x Parameter for the log-gamma function.
    \return The lgammal functions return log_e | Γ(x) |.
*/
long double lgammal(long double x);

// Nearest integer floating-point operations

//! Round up value.
/*! 
    The ceil functions compute the smallest integer value not less than x.
    \param x Value to round up.
    \return The ceil functions return ⎡x⎤, expressed as a floating-point number.
*/
double ceil(double x);

//! Round up value.
/*! 
    The ceilf functions compute the smallest integer value not less than x.
    \param x Value to round up.
    \return The ceilf functions return ⎡x⎤, expressed as a floating-point number.
*/
float ceilf(float x);

//! Round up value.
/*! 
    The ceill functions compute the smallest integer value not less than x.
    \param x Value to round up.
    \return The ceill functions return ⎡x⎤, expressed as a floating-point number.
*/
long double ceill(long double x);

//! Round down value.
/*! 
    The floor functions compute the largest integer value not greater than x.
    \param x Value to round down.
    \return The floor functions return ⎣x⎦, expressed as a floating-point number.
*/
double floor(double x);

//! Round down value.
/*! 
    The floorf functions compute the largest integer value not greater than x.
    \param x Value to round down.
    \return The floorf functions return ⎣x⎦, expressed as a floating-point number.
*/
float floorf(float x);

//! Round down value.
/*! 
    The floorl functions compute the largest integer value not greater than x.
    \param x Value to round down.
    \return The floorl functions return ⎣x⎦, expressed as a floating-point number.
*/
long double floorl(long double x);

//! Compute remainder of division.
/*! 
    The fmod functions compute the floating-point remainder of x/y.
    \param numer Value of the quotient numerator.
    \param denom Value of the quotient denominator.
    \return The fmod functions return the value x − ny, for some integer n such that, if y is nonzero, the result has the same sign as x and magnitude less than the magnitude of y. If y is zero, a domain error occurs and the fmod functions return zero.
*/
double fmod(double numer, double denom);

//! Compute remainder of division.
/*! 
    The fmodf functions compute the floating-point remainder of x/y.
    \param numer Value of the quotient numerator.
    \param denom Value of the quotient denominator.
    \return The fmodf functions return the value x − ny, for some integer n such that, if y is nonzero, the result has the same sign as x and magnitude less than the magnitude of y. If y is zero, a domain error occurs and the fmodf functions return zero.
*/
float fmodf(float numer, float denom);

//! Compute remainder of division.
/*! 
    The fmodl functions compute the floating-point remainder of x/y.
    \param numer Value of the quotient numerator.
    \param denom Value of the quotient denominator.
    \return The fmodl functions return the value x − ny, for some integer n such that, if y is nonzero, the result has the same sign as x and magnitude less than the magnitude of y. If y is zero, a domain error occurs and the fmod functions return zero.
*/
long double fmodl(long double numer, long double denom);

//! Truncate value.
/*! 
    The trunc functions round their argument to the integer value, in floating format, nearest to but no larger in magnitude than the argument.
    \param x Value to truncate.
    \return The trunc functions return the truncated integer value.
*/
double trunc(double x);

//! Truncate value.
/*! 
    The truncf functions round their argument to the integer value, in floating format, nearest to but no larger in magnitude than the argument.
    \param x Value to truncate.
    \return The truncf functions return the truncated integer value.
*/
float truncf(float x);

//! Truncate value.
/*! 
    The truncl functions round their argument to the integer value, in floating format, nearest to but no larger in magnitude than the argument.
    \param x Value to truncate.
    \return The truncl functions return the truncated integer value.
*/
long double truncl(long double x);

//! Round to nearest.
/*! 
    The round functions round their argument to the nearest integer value in floating-point format, rounding halfway cases away from zero, regardless of the current rounding direction.
    \param x Value to round.
    \return The round functions return the rounded integer value.
*/
double round(double x);

//! Round to nearest.
/*! 
    The roundf functions round their argument to the nearest integer value in floating-point format, rounding halfway cases away from zero, regardless of the current rounding direction.
    \param x Value to round.
    \return The roundf functions return the rounded integer value.
*/
float roundf(float x);

//! Round to nearest.
/*! 
    The roundl functions round their argument to the nearest integer value in floating-point format, rounding halfway cases away from zero, regardless of the current rounding direction.
    \param x Value to round.
    \return The roundl functions return the rounded integer value.
*/
long double roundl(long double x);

//! Round to nearest and cast to long integer.
/*! 
    The lround and llround functions round their argument to the nearest integer value, rounding halfway cases away from zero, regardless of the current rounding direction. If the rounded value is outside the range of the return type, the numeric result is unspecified and a domain error or range error may occur.
    \param x Value to round.
    \return The lround and llround functions return the rounded integer value.
*/
long int lround(double x);

//! Round to nearest and cast to long integer.
/*! 
    The lround and llround functions round their argument to the nearest integer value, rounding halfway cases away from zero, regardless of the current rounding direction. If the rounded value is outside the range of the return type, the numeric result is unspecified and a domain error or range error may occur.
    \param x Value to round.
    \return The lround and llround functions return the rounded integer value.
*/
long int lroundf(float x);

//! Round to nearest and cast to long integer.
/*! 
    The lroundl and llround functions round their argument to the nearest integer value, rounding halfway cases away from zero, regardless of the current rounding direction. If the rounded value is outside the range of the return type, the numeric result is unspecified and a domain error or range error may occur.
    \param x Value to round.
    \return The lroundl and llround functions return the rounded integer value.
*/
long int lroundl(long double x);

//! Round to nearest and cast to long long integer.
/*! 
    The lround and llround functions round their argument to the nearest integer value, rounding halfway cases away from zero, regardless of the current rounding direction. If the rounded value is outside the range of the return type, the numeric result is unspecified and a domain error or range error may occur.
    \param x Value to round.
    \return The lround and llround functions return the rounded integer value.
*/
long long int llround(double x);

//! Round to nearest and cast to long long integer.
/*! 
    The lround and llround functions round their argument to the nearest integer value, rounding halfway cases away from zero, regardless of the current rounding direction. If the rounded value is outside the range of the return type, the numeric result is unspecified and a domain error or range error may occur.
    \param x Value to round.
    \return The lround and llround functions return the rounded integer value.
*/
long long int llroundf(float x);

//! Round to nearest and cast to long long integer.
/*! 
    The lround and llround functions round their argument to the nearest integer value, rounding halfway cases away from zero, regardless of the current rounding direction. If the rounded value is outside the range of the return type, the numeric result is unspecified and a domain error or range error may occur.
    \param x Value to round.
    \return The lround and llround functions return the rounded integer value.
*/
long long int llroundl(long double x);

//! Round to integral value.
/*! 
    The rint functions round their argument to an integer value in floating-point format, using the current rounding direction.
    \param x Value to round.
    \return The rint functions return the rounded integer value.
*/
double rint(double x);

//! Round to integral value.
/*! 
    The rintf functions round their argument to an integer value in floating-point format, using the current rounding direction.
    \param x Value to round.
    \return The rintf functions return the rounded integer value.
*/
float rintf(float x);

//! Round to integral value.
/*! 
    The rintl functions round their argument to an integer value in floating-point format, using the current rounding direction.
    \param x Value to round.
    \return The rintl functions return the rounded integer value.
*/
long double rintl(long double x);

//! Round and cast to long integer.
/*! 
    The lrint function round their argument to the nearest integer value, rounding according to the current rounding direction. If the rounded value is outside the range of the return type, the numeric result is unspecified and a domain error or range error may occur.
    \param x Value to round.
    \return The lrint function return the rounded integer value.
*/
long int lrint(double x);

//! Round and cast to long integer.
/*! 
    The lrintf function round their argument to the nearest integer value, rounding according to the current rounding direction. If the rounded value is outside the range of the return type, the numeric result is unspecified and a domain error or range error may occur.
    \param x Value to round.
    \return The lrintf function return the rounded integer value.
*/
long int lrintf(float x);

//! Round and cast to long integer.
/*! 
    The lrintl function round their argument to the nearest integer value, rounding according to the current rounding direction. If the rounded value is outside the range of the return type, the numeric result is unspecified and a domain error or range error may occur.
    \param x Value to round.
    \return The lrintl function return the rounded integer value.
*/
long int lrintl(long double x);

//! Round and cast to long long integer.
/*! 
    The llrint function round their argument to the nearest integer value, rounding according to the current rounding direction. If the rounded value is outside the range of the return type, the numeric result is unspecified and a domain error or range error may occur.
    \param x Value to round.
    \return The llrint function return the rounded integer value.
*/
long long int llrint(double x);

//! Round and cast to long long integer.
/*! 
    The llrintf function round their argument to the nearest integer value, rounding according to the current rounding direction. If the rounded value is outside the range of the return type, the numeric result is unspecified and a domain error or range error may occur.
    \param x Value to round.
    \return The llrintf function return the rounded integer value.
*/
long long int llrintf(float x);

//! Round and cast to long long integer.
/*! 
    The llrintl function round their argument to the nearest integer value, rounding according to the current rounding direction. If the rounded value is outside the range of the return type, the numeric result is unspecified and a domain error or range error may occur.
    \param x Value to round.
    \return The llrintl function return the rounded integer value.
*/
long long int llrintl(long double x);

//! Round to nearby integral value.
/*! 
    The nearbyint functions round their argument to an integer value in floating-point format, using the current rounding direction and without raising the ‘‘inexact’’ floatingpoint exception.
    \param x Value to round.
    \return The nearbyint functions return the rounded integer value.
*/
double nearbyint(double x);

//! Round to nearby integral value.
/*! 
    The nearbyintf functions round their argument to an integer value in floating-point format, using the current rounding direction and without raising the ‘‘inexact’’ floatingpoint exception.
    \param x Value to round.
    \return The nearbyintf functions return the rounded integer value.
*/
float nearbyintf(float x);

//! Round to nearby integral value.
/*! 
    The nearbyintl functions round their argument to an integer value in floating-point format, using the current rounding direction and without raising the ‘‘inexact’’ floatingpoint exception.
    \param x Value to round.
    \return The nearbyintl functions return the rounded integer value.
*/
long double nearbyintl(long double x);

//! Compute reminder required by IEC 60559.
/*! 
    The remainder functions compute the remainder x REM y required by IEC 60559.
    \param x Value of the quotient numerator..
    \param y Value of the quotient denominator..
    \return The remainder functions return x REM y. If y is zero, whether a domain error occurs or the functions return zero is implementation defined.
*/
double remainder(double x, double y);

//! Compute reminder required by IEC 60559.
/*! 
    The remainderf functions compute the remainder x REM y required by IEC 60559.
    \param x Value of the quotient numerator..
    \param y Value of the quotient denominator..
    \return The remainder functions return x REM y. If y is zero, whether a domain error occurs or the functions return zero is implementation defined.
*/
float remainderf(float x, float y);

//! Compute reminder required by IEC 60559.
/*! 
    The remainderl functions compute the remainder x REM y required by IEC 60559.
    \param x Value of the quotient numerator..
    \param y Value of the quotient denominator..
    \return The remainderl functions return x REM y. If y is zero, whether a domain error occurs or the functions return zero is implementation defined.
*/
long double remainderl(long double x, long double y);

//! Compute remainder and quotient.
/*! 
    The remquo functions compute the same remainder as the remainder functions. In the object pointed to by quo they store a value whose sign is the sign of x/y and whose magnitude is congruent modulo 2n to the magnitude of the integral quotient of x/y, where n is an implementation-defined integer greater than or equal to 3.
    \param numer Floating point value with the quotient numerator.
    \param denom Floating point value with the quotient denominator.
    \param quot Pointer to an object where the quotient internally used to determine the remainder is stored as a value of type int.
    \return The remquo functions return x REM y. If y is zero, the value stored in the object pointed to by quo is unspecified and whether a domain error occurs or the functions return zero is implementation defined.
*/
double remquo(double numer, double denom, int* quot);

//! Compute remainder and quotient.
/*! 
    The remquof functions compute the same remainder as the remainder functions. In the object pointed to by quo they store a value whose sign is the sign of x/y and whose magnitude is congruent modulo 2n to the magnitude of the integral quotient of x/y, where n is an implementation-defined integer greater than or equal to 3.
    \param numer Floating point value with the quotient numerator.
    \param denom Floating point value with the quotient denominator.
    \param quot Pointer to an object where the quotient internally used to determine the remainder is stored as a value of type int.
    \return The remquof functions return x REM y. If y is zero, the value stored in the object pointed to by quo is unspecified and whether a domain error occurs or the functions return zero is implementation defined.
*/
float remquof(float numer, float denom, int* quot);

//! Compute remainder and quotient.
/*! 
    The remquol functions compute the same remainder as the remainder functions. In the object pointed to by quo they store a value whose sign is the sign of x/y and whose magnitude is congruent modulo 2n to the magnitude of the integral quotient of x/y, where n is an implementation-defined integer greater than or equal to 3.
    \param numer Floating point value with the quotient numerator.
    \param denom Floating point value with the quotient denominator.
    \param quot Pointer to an object where the quotient internally used to determine the remainder is stored as a value of type int.
    \return The remquol functions return x REM y. If y is zero, the value stored in the object pointed to by quo is unspecified and whether a domain error occurs or the functions return zero is implementation defined.
*/
long double remquol(long double numer, long double denom, int* quot);

// Floating-point manipulation functions

//! Copy sign.
/*! 
    The copysign functions produce a value with the magnitude of x and the sign of y. They produce a NaN (with the sign of y) if x is a NaN. On implementations that represent a signed zero but do not treat negative zero consistently in arithmetic operations, the copysign functions regard the sign of zero as positive.
    \param x Value with the magnitude of the resulting value.
    \param y Value with the sign of the resulting value.
    \return The copysign functions return a value with the magnitude of x and the sign of y.
*/
double copysign(double x, double y);

//! Copy sign.
/*! 
    The copysignf functions produce a value with the magnitude of x and the sign of y. They produce a NaN (with the sign of y) if x is a NaN. On implementations that represent a signed zero but do not treat negative zero consistently in arithmetic operations, the copysign functions regard the sign of zero as positive.
    \param x Value with the magnitude of the resulting value.
    \param y Value with the sign of the resulting value.
    \return The copysignf functions return a value with the magnitude of x and the sign of y.
*/  
float copysignf(float x, float y);

//! Copy sign.
/*! 
    The copysignl functions produce a value with the magnitude of x and the sign of y. They produce a NaN (with the sign of y) if x is a NaN. On implementations that represent a signed zero but do not treat negative zero consistently in arithmetic operations, the copysign functions regard the sign of zero as positive.
    \param x Value with the magnitude of the resulting value.
    \param y Value with the sign of the resulting value.
    \return The copysignl functions return a value with the magnitude of x and the sign of y.
*/  
long double copysignl(long double x, long double y);

//! Generate quiet NaN.
/*! 
    The call nan("n-char-sequence") is equivalent to strtod("NAN(n-charsequence)", (char**) NULL); the call nan("") is equivalent to strtod("NAN()", (char**) NULL). If tagp does not point to an n-char sequence or an empty string, the call is equivalent to strtod("NAN", (char**) NULL). Calls to nanf and nanl are equivalent to the corresponding calls to strtof and strtold.
    \param arg Narrow character string identifying the contents of a NaN.
    \return The nan functions return a quiet NaN, if available, with content indicated through tagp. If the implementation does not support quiet NaNs, the functions return zero.
*/
double nan(const char* arg);

//! Generate quiet NaN.
/*! 
    The call nan("n-char-sequence") is equivalent to strtod("NAN(n-charsequence)", (char**) NULL); the call nan("") is equivalent to strtod("NAN()", (char**) NULL). If tagp does not point to an n-char sequence or an empty string, the call is equivalent to strtod("NAN", (char**) NULL). Calls to nanf and nanl are equivalent to the corresponding calls to strtof and strtold.
    \param arg Narrow character string identifying the contents of a NaN.
    \return The nanf functions return a quiet NaN, if available, with content indicated through tagp. If the implementation does not support quiet NaNs, the functions return zero.
*/
float nanf(const char* arg);

//! Generate quiet NaN.
/*! 
    The call nan("n-char-sequence") is equivalent to strtod("NAN(n-charsequence)", (char**) NULL); the call nan("") is equivalent to strtod("NAN()", (char**) NULL). If tagp does not point to an n-char sequence or an empty string, the call is equivalent to strtod("NAN", (char**) NULL). Calls to nanf and nanl are equivalent to the corresponding calls to strtof and strtold.
    \param arg Narrow character string identifying the contents of a NaN.
    \return The nanl functions return a quiet NaN, if available, with content indicated through tagp. If the implementation does not support quiet NaNs, the functions return zero.
*/
long double nanl(const char* arg);

//! Next representable value.
/*! 
    The nextafter functions determine the next representable value, in the type of the function, after x in the direction of y, where x and y are first converted to the type of the function. The nextafter functions return y if x equals y. A range error may occur if the magnitude of x is the largest finite value representable in the type and the result is infinite or not representable in the type.
    \param x Base value.
    \param y Value toward which the return value is approximated.
    \return The nextafter functions return the next representable value in the specified format after x in the direction of y.
*/
double nextafter(double x, double y);

//! Next representable value.
/*! 
    The nextafterf functions determine the next representable value, in the type of the function, after x in the direction of y, where x and y are first converted to the type of the function. The nextafterf functions return y if x equals y. A range error may occur if the magnitude of x is the largest finite value representable in the type and the result is infinite or not representable in the type.
    \param x Base value.
    \param y Value toward which the return value is approximated.
    \return The nextafterf functions return the next representable value in the specified format after x in the direction of y.
*/
float nextafterf(float x, float y);

//! Next representable value.
/*! 
    The nextafterl functions determine the next representable value, in the type of the function, after x in the direction of y, where x and y are first converted to the type of the function. The nextafterl functions return y if x equals y. A range error may occur if the magnitude of x is the largest finite value representable in the type and the result is infinite or not representable in the type.
    \param x Base value.
    \param y Value toward which the return value is approximated.
    \return The nextafterl functions return the next representable value in the specified format after x in the direction of y.
*/
long double nextafterl(long double x, long double y);

//! Next representable value toward precise value.
/*! 
    The nexttoward functions are equivalent to the nextafter functions except that the second parameter has type long double and the functions return y converted to the type of the function if x equals y.
    \param x Base value.
    \param y Value toward which the return value is approximated.
    \return The next representable value after x in the direction of y.
*/
double nexttoward(double x, long double y);

//! Next representable value toward precise value.
/*! 
    The nexttowardf functions are equivalent to the nextafter functions except that the second parameter has type long double and the functions return y converted to the type of the function if x equals y.
    \param x Base value.
    \param y Value toward which the return value is approximated.
    \return The next representable value after x in the direction of y.
*/
float nexttowardf(float x, long double y);

//! Next representable value toward precise value.
/*! 
    The nexttowardl functions are equivalent to the nextafter functions except that the second parameter has type long double and the functions return y converted to the type of the function if x equals y.
    \param x Base value.
    \param y Value toward which the return value is approximated.
    \return The next representable value after x in the direction of y.
*/
long double nexttowardl(long double x, long double y);

// Minimum, maximum, difference functions

//! Positive difference.
/*! 
    The fdim functions determine the positive difference between their arguments.
    \param x Value whose difference is calculated.
    \param y Value whose difference is calculated.
    \return The fdim functions return the positive difference value.
*/
double fdim(double x, double y);

//! Positive difference.
/*! 
    The fdimf functions determine the positive difference between their arguments.
    \param x Value whose difference is calculated.
    \param y Value whose difference is calculated.
    \return The fdimf functions return the positive difference value.
*/
float fdimf(float x, float y);

//! Positive difference.
/*! 
    The fdiml functions determine the positive difference between their arguments.
    \param x Value whose difference is calculated.
    \param y Value whose difference is calculated.
    \return The fdiml functions return the positive difference value.
*/
long double fdiml(long double x, long double y);

//! Higher value.
/*! 
    The fmax functions determine the maximum numeric value of their arguments.
    \param x Value among which the function selects a maximum.
    \param y Value among which the function selects a maximum.
    \return The fmax functions return the maximum numeric value of their arguments.
*/
double fmax(double x, double y);

//! Higher value.
/*! 
    The fmaxf functions determine the maximum numeric value of their arguments.
    \param x Value among which the function selects a maximum.
    \param y Value among which the function selects a maximum.
    \return The fmaxf functions return the maximum numeric value of their arguments.
*/
float fmaxf(float x, float y);

//! Higher value.
/*! 
    The fmaxl functions determine the maximum numeric value of their arguments.
    \param x Value among which the function selects a maximum.
    \param y Value among which the function selects a maximum.
    \return The fmaxl functions return the maximum numeric value of their arguments.
*/
long double fmaxl(long double x, long double y);

//! Lower value.
/*! 
    The fmin functions determine the minimum numeric value of their arguments.
    \param x Value among which the function selects a minimum.
    \param y Value among which the function selects a minimum.
    \return The fmin functions return the minimum numeric value of their arguments.
*/
double fmin(double x, double y);

//! Lower value.
/*! 
    The fminf functions determine the minimum numeric value of their arguments.
    \param x Value among which the function selects a minimum.
    \param y Value among which the function selects a minimum.
    \return The fminf functions return the minimum numeric value of their arguments.
*/
float fminf(float x, float y);

//! Lower value.
/*! 
    The fminl functions determine the minimum numeric value of their arguments.
    \param x Value among which the function selects a minimum.
    \param y Value among which the function selects a minimum.
    \return The fminl functions return the minimum numeric value of their arguments.
*/
long double fminl(long double x, long double y);

// Other functions

//! Compute absolute value.
/*! 
    The fabs functions compute the absolute value of a floating-point number x.
    \param x Value among which the function selects a minimum.
    \return The fabs functions return | x |.
*/
double fabs(double x);

//! Compute absolute value.
/*! 
    The fabsf functions compute the absolute value of a floating-point number x.
    \param x Value among which the function selects a minimum.
    \return The fabsf functions return | x |.
*/
float fabsf(float x);

//! Compute absolute value.
/*! 
    The fabsl functions compute the absolute value of a floating-point number x.
    \param x Value among which the function selects a minimum.
    \return The fabsl functions return | x |.
*/
long double fabsl(long double x);

//! Multiply-add.
/*! 
    The fma functions compute (x × y) + z, rounded as one ternary operation: they compute the value (as if) to infinite precision and round once to the result format, according to the current rounding mode. A range error may occur.
    \param x Value to be multiplied.
    \param y Value to be multiplied.
    \param z Value to be added.
    \return The fma functions return (x × y) + z, rounded as one ternary operation.
*/
double fma(double x, double y, double z);

//! Multiply-add.
/*! 
    The fmaf functions compute (x × y) + z, rounded as one ternary operation: they compute the value (as if) to infinite precision and round once to the result format, according to the current rounding mode. A range error may occur.
    \param x Value to be multiplied.
    \param y Value to be multiplied.
    \param z Value to be added.
    \return The fmaf functions return (x × y) + z, rounded as one ternary operation.
*/
float fmaf(float x, float y, float z);

//! Multiply-add.
/*! 
    The fmal functions compute (x × y) + z, rounded as one ternary operation: they compute the value (as if) to infinite precision and round once to the result format, according to the current rounding mode. A range error may occur.
    \param x Value to be multiplied.
    \param y Value to be multiplied.
    \param z Value to be added.
    \return The fmal functions return (x × y) + z, rounded as one ternary operation.
*/
long double fmal(long double x, long double y, long double z);

#ifdef __cplusplus
}
#endif

//! Type represent float value as array of bytes.
typedef union float_byte
{
    //! Float value.
    float f;
    //! Array of bytes.
    unsigned char bytes[sizeof(float)];
} float_byte;

//! Type represent double value as array of bytes.
typedef union double_byte
{
    //! Double value.
    double f;
    //! Array of bytes.
    unsigned char bytes[sizeof(double)];
} double_byte;

//! Type represent long double value as array of bytes.
typedef union long_double_byte
{
    //! Long double value.
    long double f;
    //! Array of bytes.
    unsigned char bytes[sizeof(long double)];
} long_double_byte;

#ifdef __cplusplus
extern "C" {
#endif

// Additional content

//! Classify floating-point value.
/*! 
    Returns a value of type int that matches one of the classification macro constants, depending on the value of x.
    \param x The value to classify.
    \return One of the followoing int values: FP_INFINITE, FP_NAN, FP_ZERO, FP_SUBNORMAL, FP_NORMAL, FP_WTF.
*/
int __math_fpclasify(double x);

//! Classify floating-point value.
/*! 
    Returns a value of type int that matches one of the classification macro constants, depending on the value of x.
    \param x The value to classify.
    \return One of the followoing int values: FP_INFINITE, FP_NAN, FP_ZERO, FP_SUBNORMAL, FP_NORMAL, FP_WTF.
*/
int __math_fpclasifyf(float x);

//! Classify floating-point value.
/*! 
    Returns a value of type int that matches one of the classification macro constants, depending on the value of x.
    \param x The value to classify.
    \return One of the followoing int values: FP_INFINITE, FP_NAN, FP_ZERO, FP_SUBNORMAL, FP_NORMAL, FP_WTF.
*/
int __math_fpclasifyl(long double x);

//! The signbit determines whether the sign of its argument value is negative.
/*!
    \param arg The value to check.
    \return The signbit returns a nonzero value if and only if the sign of its argument value is negative.
*/
int __math_signbit(double x);

//! The signbit determines whether the sign of its argument value is negative.
/*!
    \param arg The value to check.
    \return The signbit returns a nonzero value if and only if the sign of its argument value is negative.
*/
int __math_signbitf(float x);

//! The signbit determines whether the sign of its argument value is negative.
/*!
    \param arg The value to check.
    \return The signbit returns a nonzero value if and only if the sign of its argument value is negative.
*/
int __math_signbitl(long double x);

//! Returns quiet NAN.
/*! 
    \return Quiet NAN.
*/
float __math_NANf();

//! Returns infinity.
/*! 
    \return Infinity.
*/
double __math_INF();

//! Returns infinity.
/*! 
    \return Infinity.
*/
float __math_INFf();

//! Returns infinity.
/*! 
    \return Infinity.
*/
long double __math_INFl();

#ifdef __cplusplus
}
#endif

//! Classify floating-point value.
/*! 
    Returns a value of type int that matches one of the classification macro constants, depending on the value of x.
    \param x The value to classify.
    \return One of the followoing int values: FP_INFINITE, FP_NAN, FP_ZERO, FP_SUBNORMAL, FP_NORMAL, FP_WTF.
*/
#define fpclasify(x) (sizeof(x) == sizeof(double) ? __math_fpclasify(x) : (sizeof(x) == sizeof(float) ? __math_fpclasifyf(x) : __math_fpclasifyl(x)))

//! Is finite.
/*! 
    Returns whether x is a finite value. A finite value is any floating-point value that is neither infinite nor NaN (Not-A-Number).
    \param x The value to check.
    \return 1 if true, 0 if false
*/
#define isfinite(x) ((fpclasify(x) == FP_NORMAL || fpclasify(x) == FP_ZERO ))

//! Is infinity.
/*! 
    Returns whether x is an infinity value (either positive infinity or negative infinity).
    \param x The value to check.
    \return 1 if true, 0 if false
*/
#define isinf(x) (fpclasify(x) == FP_INFINITE)

//! Is Not-A-Number.
/*! 
    Returns whether x is a NaN (Not-A-Number) value.
    \param x The value to check.
    \return 1 if true, 0 if false
*/
#define isnan(x) (fpclasify(x) == FP_NAN)

//! Is normal.
/*! 
    Returns whether x is a normal value: i.e., whether it is neither infinity, NaN, zero or subnormal.
    \param x The value to check.
    \return 1 if true, 0 if false
*/
#define isnormal(x) (fpclasify(x) == FP_NORMAL)

//! The signbit macro determines whether the sign of its argument value is negative.
/*!
    \param arg The value to check.
    \return The signbit macro returns a nonzero value if and only if the sign of its argument value is negative.
*/
#define signbit(arg) (sizeof(arg) == sizeof(double) ? __math_signbit(arg) : (sizeof(arg) == sizeof(float) ? __math_signbitf(arg) : __math_signbitl(arg)))

//! Multiply-add.
/*! 
    Compute (x × y) + z, rounded as one ternary operation: they compute the value (as if) to infinite precision and round once to the result format, according to the current rounding mode. A range error may occur.
    \param x Value to be multiplied.
    \param y Value to be multiplied.
    \param z Value to be added.
    \return Return (x × y) + z, rounded as one ternary operation.
*/
#define FP_FAST_FMA(x, y, z) (fma(x, y, z))

//! Multiply-add.
/*! 
    Compute (x × y) + z, rounded as one ternary operation: they compute the value (as if) to infinite precision and round once to the result format, according to the current rounding mode. A range error may occur.
    \param x Value to be multiplied.
    \param y Value to be multiplied.
    \param z Value to be added.
    \return Return (x × y) + z, rounded as one ternary operation.
*/
#define FP_FAST_FMAF(x, y, z) (fmaf(x, y, z))

//! Multiply-add.
/*! 
    Compute (x × y) + z, rounded as one ternary operation: they compute the value (as if) to infinite precision and round once to the result format, according to the current rounding mode. A range error may occur.
    \param x Value to be multiplied.
    \param y Value to be multiplied.
    \param z Value to be added.
    \return Return (x × y) + z, rounded as one ternary operation.
*/
#define FP_FAST_FMAL(x, y, z) (fmal(x, y, z))

//! Returns quiet NAN.
/*! 
    \return Quiet NAN.
*/
#define NAN (__math_NANf())

//! Returns infinity.
/*! 
    \return Infinity.
*/
#define INFINITY (__math_INFf())

//! Returns infinity.
/*! 
    \return Infinity.
*/
#define HUGE_VAL (__math_INF())

//! Returns infinity.
/*! 
    \return Infinity.
*/
#define HUGE_VALF (__math_INFf())

//! Returns infinity.
/*! 
    \return Infinity.
*/
#define HUGE_VALL (__math_INFl())

#endif //MATH_H