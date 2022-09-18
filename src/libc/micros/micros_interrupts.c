#include "micros_interrupts.h"

uint32_t micros_interrupt_0a(uint32_t function_number)
{
#ifndef __libk 

    uint32_t result;
    __asm__(
        "mov %1, %%eax\n"
        "int $50\n"
        "mov %%eax, %0\n"

        : "=g"(result)
        : "g"(function_number)
        : "%eax");

    return result;

#else

#endif
}

uint32_t micros_interrupt_1a(uint32_t function_number, uint32_t a)
{
#ifndef __libk 

    uint32_t result;
    __asm__(
        "mov %1, %%eax\n"
        "mov %2, %%ebx\n"
        "int $50\n"
        "mov %%eax, %0\n"

        : "=g"(result)
        : "g"(function_number), "g"(a)
        : "%eax", "%ebx");

    return result;

#else

#endif
}

uint32_t micros_interrupt_2a(uint32_t function_number, uint32_t a, uint32_t b)
{
#ifndef __libk 

    uint32_t result;
    __asm__(
        "mov %1, %%eax\n"
        "mov %2, %%ebx\n"
        "mov %3, %%ecx\n"
        "int $50\n"
        "mov %%eax, %0\n"

        : "=g"(result)
        : "g"(function_number), "g"(a), "g"(b)
        : "%eax", "%ebx", "%ecx");

    return result;

#else

#endif
}

uint32_t micros_interrupt_3a(uint32_t function_number, uint32_t a, uint32_t b, uint32_t c)
{
#ifndef __libk 

    uint32_t result;
    __asm__(
        "mov %1, %%eax\n"
        "mov %2, %%ebx\n"
        "mov %3, %%ecx\n"
        "mov %4, %%edx\n"
        "int $50\n"
        "mov %%eax, %0\n"

        : "=g"(result)
        : "g"(function_number), "g"(a), "g"(b), "g"(c)
        : "%eax", "%ebx", "%ecx", "%edx");

    return result;

#else

#endif
}

uint32_t micros_interrupt_4a(uint32_t function_number, uint32_t a, uint32_t b, uint32_t c, uint32_t d)
{
#ifndef __libk 

    uint32_t result;
    __asm__(
        "mov %1, %%eax\n"
        "mov %2, %%ebx\n"
        "mov %3, %%ecx\n"
        "mov %4, %%edx\n"
        "mov %5, %%esi\n"
        "int $50\n"
        "mov %%eax, %0\n"
        : "=g"(result)
        : "g"(function_number), "g"(a), "g"(b), "g"(c), "g"(d)
        : "%eax", "%ebx", "%ecx", "%edx", "%esi");

    return result;

#else

#endif
}

uint32_t micros_interrupt_5a(uint32_t function_number, uint32_t a, uint32_t b, uint32_t c, uint32_t d, uint32_t e)
{
#ifndef __libk 

    uint32_t result;
    __asm__(
        "mov %1, %%eax\n"
        "mov %2, %%ebx\n"
        "mov %3, %%ecx\n"
        "mov %4, %%edx\n"
        "mov %5, %%esi\n"
        "mov %6, %%edi\n"
        "int $50\n"
        "mov %%eax, %0\n"

        : "=g"(result)
        : "g"(function_number), "g"(a), "g"(b), "g"(c), "g"(d), "g"(e)
        : "%eax", "%ebx", "%ecx", "%edx", "%esi", "%edi");

    return result;

#else

#endif
}