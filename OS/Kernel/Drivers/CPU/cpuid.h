#ifndef CPUID_H
#define CPUID_H

#include <stdint.h>

static inline void cpuid(unsigned int code, uint32_t where[4]) {
  asm volatile("cpuid":"=a"(*where),"=b"(*(where+1)),
               "=c"(*(where+2)),"=d"(*(where+3)):"a"(code));
}

void printCpu();
void printBrand();

#endif