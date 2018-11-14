#ifndef STDLIB_H
#define STDLIB_H

#include <stdint.h>

void sleep(uint32_t ms);
char *itoa(int input, char *buffer, int base);

#endif