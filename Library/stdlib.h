#ifndef STDLIB_H
#define STDLIB_H

#include <stdint.h>
#include "string.h"
#include "interrupt_helper.h"
#include "time.h"

void sleep(uint32_t ms);
int32_t atoi(const char* string);
char* itoa(int input, char* buffer, int base);

void* malloc(size_t size);
void* calloc(size_t num, size_t size);
void* realloc(void* ptr, size_t size);
void free(void* ptr);

#endif