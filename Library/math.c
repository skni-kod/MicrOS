#include "math.h"

double fabs(double x)
{
    __asm__ ("fldl  %1 \n fabs \n fstpl %0" : "=m"(x): "m"(x));
    return x;
}

float fabsf(float x)
{
    __asm__ ("fld  %1 \n fabs \n fstp %0" : "=m"(x): "m"(x));
    return x;
}

// Power functions 

double sqrt(double x)
{
    __asm__ ("fldl  %1 \n fsqrt \n fstpl %0" : "=m"(x): "m"(x));
    return x;  
}

float sqrtf(float x)
{
    __asm__ ("fld  %1 \n fsqrt \n fstp %0" : "=m"(x): "m"(x));
    return x;
}

//Trigonometric functions

double sin(double x)
{
    __asm__ ("fldl  %1 \n fsin \n fstpl %0" : "=m"(x): "m"(x));
    return x;
}

float sinf(float x)
{
    __asm__ ("fld  %1 \n fsin \n fstp %0" : "=m"(x): "m"(x));
    return x;
}

double cos(double x)
{
    __asm__ ("fldl  %1 \n fcos \n fstpl %0" : "=m"(x): "m"(x));
    return x;
}

float cosf(float x)
{
    __asm__ ("fld  %1 \n fcos \n fstp %0" : "=m"(x): "m"(x));
    return x;
}

double tan(double x)
{
    __asm__ ("fldl  %1 \n fptan \n fstpl %0 \n fstpl %0" : "=m"(x): "m"(x));
    return x;
}

float tanf(float x)
{
    __asm__ ("fld  %1 \n fptan \n fstp %0 \n fstp %0" : "=m"(x): "m"(x));
    return x;
}

double asin(double x)
{
    __asm__ ("fldl  %1 \n fldl  %1 \n fmulp \n fld1 \n fsubp \n fsqrt \n fldl  %1 \n fdivp \n fld1 \n fpatan \n fstpl %0" : "=m"(x): "m"(x));
    return x;
}

float asinf(float x)
{
    __asm__ ("fld  %1 \n fld  %1 \n fmulp \n fld1 \n fsubp \n fsqrt \n fld  %1 \n fdivp \n fld1 \n fpatan \n fstp %0" : "=m"(x): "m"(x));
    return x;
}

double acos(double x)
{
    double two = 2;
    double asinX = asin(x);
    __asm__ ("fldl %1 \n fldpi \n fdivp \n fldl %2 \n fsubrp \n fstpl %0" : "=m"(x): "m"(two), "m"(asinX));
    return x;
}

float acosf(float x)
{
    float two = 2;
    float asinX = asinf(x);
    __asm__ ("fld %1 \n fldpi \n fdivp \n fld %2 \n fsubrp \n fstp %0" : "=m"(x): "m"(two), "m"(asinX));
    return x;
}

//Nearest integer floating-point operations

double round(double x)
{
    __asm__ ("fldl  %1 \n frndint \n fstpl %0" : "=m"(x): "m"(x));
    return x;
}

float roundf(float x)
{
    __asm__ ("fld  %1 \n frndint \n fstp %0" : "=m"(x): "m"(x));
    return x;
}