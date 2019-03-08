#ifndef STDIO_H
#define STDIO_H

#define BUFSIZ 1024 * 8
#define size_t uint32_t

#include <stdint.h>
#include "Streams/file.h"
#include "Streams/console_stream.h"
#include "Streams/keyboard_stream.h"

FILE *stdin;
FILE *stdout;
FILE *stderr;

int fclose(FILE *stream);
int fflush(FILE *stream);

int fgetc(FILE *stream);
char *fgets(char *str, int num, FILE *stream);
int fputc(int character, FILE *stream);
int fputs(const char *str, FILE *stream);
int getc(FILE *stream);
int getchar();
char *gets(char *str);
int putc(int character, FILE *stream);
int putchar(int character);
int puts(const char *str);
int ungetc(int character, FILE *stream);

size_t fwrite(const void *ptr, size_t size, size_t count, FILE *stream);

FILE *streams_create_stream();

#endif