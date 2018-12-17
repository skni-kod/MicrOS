#ifndef STDLIB_H
#define STDLIB_H

#include <stdint.h>
#include "string.h"
#include "time.h"
#include "Helpers/interrupt_helper.h"

void sleep(uint32_t ms);
int32_t atoi(const char *string);
char *itoa(int input, char *buffer, int base);

void *malloc(size_t size);
void *malloc_align(size_t size, uint32_t align);
void *calloc(size_t num, size_t size);
void *realloc(void *ptr, size_t size);
void free(void *ptr);

#endif