#include "micros_interrupts.h"

uint32_t micros_interrupt_0a(uint32_t function_number)
{
    uint32_t result;
    __asm__(
        "mov %1, %%eax\n"
        "int $48\n"
        "mov %%eax, %0\n"

        : "=g"(result)
        : "g"(function_number)
        : "%eax");

    return result;
}

uint32_t micros_interrupt_1a(uint32_t function_number, uint32_t a)
{
    uint32_t result;
    __asm__(
        "mov %1, %%eax\n"
        "mov %2, %%ebx\n"
        "int $48\n"
        "mov %%eax, %0\n"

        : "=g"(result)
        : "g"(function_number), "g"(a)
        : "%eax", "%ebx");

    return result;
}

uint32_t micros_interrupt_2a(uint32_t function_number, uint32_t a, uint32_t b)
{
    uint32_t result;
    __asm__(
        "mov %1, %%eax\n"
        "mov %2, %%ebx\n"
        "mov %3, %%ecx\n"
        "int $48\n"
        "mov %%eax, %0\n"

        : "=g"(result)
        : "g"(function_number), "g"(a), "g"(b)
        : "%eax", "%ebx", "%ecx");

    return result;
}

uint32_t micros_interrupt_3a(uint32_t function_number, uint32_t a, uint32_t b, uint32_t c)
{
    uint32_t result;
    __asm__(
        "mov %1, %%eax\n"
        "mov %2, %%ebx\n"
        "mov %3, %%ecx\n"
        "mov %4, %%edx\n"
        "int $48\n"
        "mov %%eax, %0\n"

        : "=g"(result)
        : "g"(function_number), "g"(a), "g"(b), "g"(c)
        : "%eax", "%ebx", "%ecx", "%edx");

    return result;
}