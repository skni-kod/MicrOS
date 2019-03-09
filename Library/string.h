#ifndef STRING_H
#define STRING_H

#include <stdint.h>
#include <stddef.h>

typedef size_t uint32_t;

void *memcpy(void *destination, const void *source, size_t size);
void *memmove(void *destination, const void *source, size_t size);
char *strcpy(char *destination, const char *source);
char *strncpy(char *destination, const char *source, size_t num);

char *strcat(char *destination, const char *source);
char *strncat(char *destination, const char *source, size_t num);

int memcmp(const void *buffer1, const void *buffer2, size_t size);
int strcmp(const char *str1, const char *str2);
int strcoll(const char *str1, const char *str2);
int strncmp(const char *str1, const char *str2, size_t num);
size_t strxfrm(char *destination, const char *source, size_t num);

void *memchr(void *ptr, int value, size_t num);
char *strchr(char *str, int character);
size_t strcspn(const char *str1, const char *str2);
char *strpbrk(char *str1, const char *str2);
char *strrchr(char *str, int character);
size_t strspn(const char *str1, const char *str2);
char *strstr(char *str1, const char *str2);
char *strtok(char *str, const char *delimiters);

void *memset(void *buffer, int value, size_t size);
char *strerror(int errnum);
size_t strlen(const char *str);

#endif