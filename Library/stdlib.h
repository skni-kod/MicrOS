#ifndef STDLIB_H
#define STDLIB_H

#define RAND_MAX INT32_MAX

#include <stdint.h>
#include "string.h"
#include "time.h"
#include "micros.h"

int32_t atoi(const char *string);
char *itoa(int input, char *buffer, int base);

void *malloc(size_t size);
void *malloc_align(size_t size, uint32_t align);
void *calloc(size_t num, size_t size);
void *realloc(void *ptr, size_t size);
void free(void *ptr);

int rand();
void srand(unsigned int new_seed);

void exit(int status);

#endif