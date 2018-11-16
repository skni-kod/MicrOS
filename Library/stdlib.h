#ifndef STDLIB_H
#define STDLIB_H

#include <stdint.h>

int32_t atoi(const char* string);
char* itoa(int input, char* buffer, int base);
void sleep(uint32_t ms);
char *itoa(int input, char *buffer, int base);

#endif