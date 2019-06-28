#ifndef STDLIB_H
#define STDLIB_H

#define RAND_MAX INT32_MAX

#include <stdint.h>
#include "string.h"
#include "time.h"
#include "micros.h"

unsigned int seed;

#ifdef __cplusplus
extern "C" {
#endif

int32_t atoi(const char *string);
char *itoa(int input, char *buffer, int base);


//! Convert string to double
/*
    Parses the C-string str interpreting its content as a floating point number (according to the current locale) and returns its value as a double. 
    If endptr is not a null pointer, the function also sets the value of endptr to point to the first character after the number
    \param C-string beginning with the representation of a floating-point number.
    \paramReference to an already allocated object of type char*, whose value is set by the function to the next character in str after the numerical value. This parameter can also be a null pointer, in which case it is not used.
    \returnOn success, the function returns the converted floating point number as a value of type double. If no valid conversion could be performed, the function returns zero (0.0).
 */
double strtod(const char* str, char** endptr);

void *malloc(size_t size);
void *malloc_align(size_t size, uint32_t align);
void *calloc(size_t num, size_t size);
void *realloc(void *ptr, size_t size);
void free(void *ptr);

int rand();
void srand(unsigned int new_seed);

void abort();
void exit(int status);

#ifdef __cplusplus
}
#endif

#endif