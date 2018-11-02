#ifndef STRING_H
#define STRING_H

#include <stdint.h>
#include <stddef.h>

void* memset(void* buffer, int value, size_t size);
int memcmp(const void* buffer1, const void* buffer2, size_t size);
void* memcpy(void* destination, const void* source, size_t size);
void* memmove(void* destination, const void* source, size_t size);
size_t strlen(const char* str);

#endif