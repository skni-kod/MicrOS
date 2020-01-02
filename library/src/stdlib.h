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

//! Convert string to integer
/*
    Parses the C-string str interpreting its content as an integral number, which is returned as a value of type int.
    \param string C-string beginning with the representation of an integral number.
    \return On success, the function returns the converted integral number as an int value.
 */
int32_t atoi(const char *string);

//! Convert string to floating point
/*
    Parses the C-string str interpreting its content as an integral number, which is returned as a value of type int.
    \param string C-string beginning with the representation of an integral number.
    \return On success, the function returns the converted integral number as an int value.
 */
float atof(const char *string);

//! Convert integer to string (non-standard function)
/*
    Converts an integer value to a null-terminated string using the specified base and stores the result in the array given by str parameter.
    \param input Value to be converted to a string.
    \param buffer Array in memory where to store the resulting null-terminated string.
    \param base Numerical base used to represent the value as a string, between 2 and 36, where 10 means decimal base, 16 hexadecimal, 8 octal, and 2 binary.
    \return A pointer to the resulting null-terminated string, same as parameter str.
 */
char *itoa(int input, char *buffer, int base);

//! Convert string to double
/*
    Parses the C-string str interpreting its content as a floating point number (according to the current locale) and returns its value as a double. 
    If endptr is not a null pointer, the function also sets the value of endptr to point to the first character after the number
    \param str C-string beginning with the representation of a floating-point number.
    \param endptr Reference to an already allocated object of type char*, whose value is set by the function to the next character in str after the numerical value. This parameter can also be a null pointer, in which case it is not used.
    \return On success, the function returns the converted floating point number as a value of type double. If no valid conversion could be performed, the function returns zero (0.0).
 */

double strtod(const char* str, char** endptr);

//! Allocate memory block
/*
    Allocates a block of size bytes of memory, returning a pointer to the beginning of the block. The content of the newly allocated block of memory is not initialized, remaining with indeterminate values.
    \param size Size of the memory block, in bytes. size_t is an unsigned integral type.
    \return On success, a pointer to the memory block allocated by the function. If the function failed to allocate the requested block of memory, a null pointer is returned.
 */
void *malloc(size_t size);

//! Allocate memory block with the specified align
/*
    Allocates a block of size bytes of memory with the specified align, returning a pointer to the beginning of the block. The content of the newly allocated block of memory is not initialized, remaining with indeterminate values.
    \param size Size of the memory block, in bytes. size_t is an unsigned integral type.
    \param align Alignment of the block (eg. if align is set to 0x1000, then block will be allocated on addresses like 0x01110000, 0x01111000, 0x01112000,...).
    \return On success, a pointer to the memory block allocated by the function. If the function failed to allocate the requested block of memory, a null pointer is returned.
 */
void *malloc_align(size_t size, uint32_t align);

//! Allocate and zero-initialize array
/*
    Allocates a block of memory for an array of num elements, each of them size bytes long, and initializes all its bits to zero.
    \param num Number of elements to allocate.
    \param size Size of each element.
    \return On success, a pointer to the memory block allocated by the function. If the function failed to allocate the requested block of memory, a null pointer is returned.
 */
void *calloc(size_t num, size_t size);

//! Reallocate memory block
/*
    Changes the size of the memory block pointed to by ptr. The function may move the memory block to a new location (whose address is returned by the function).
    \param ptr Pointer to a memory block previously allocated with malloc, calloc or realloc.
    \param size New size for the memory block, in bytes. size_t is an unsigned integral type.
    \return A pointer to the reallocated memory block, which may be either the same as ptr or a new location.
 */
void *realloc(void *ptr, size_t size);

//! Deallocate memory block
/*
    A block of memory previously allocated by a call to malloc, calloc or realloc is deallocated, making it available again for further allocations.
    \param ptr Pointer to a memory block previously allocated with malloc, calloc or realloc.
 */
void free(void *ptr);

//! Generate random number
/*
    Returns a pseudo-random integral number in the range between 0 and RAND_MAX.
    \return An integer value between 0 and RAND_MAX.
 */
int rand();

//! Initialize random number generator
/*
    The pseudo-random number generator is initialized using the argument passed as seed.
    \param new_seed An integer value to be used as seed by the pseudo-random number generator algorithm.
 */
void srand(unsigned int new_seed);

//! Abort current process
/*
    Aborts the current process, producing an abnormal program termination.
 */
void abort();

//! Terminate calling process
/*
    Terminates the process normally, performing the regular cleanup for terminating programs.
    \param status Status code.
 */
void exit(int status);

#ifdef __cplusplus
}
#endif

#endif