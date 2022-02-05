#ifndef KERNEL_STDIO_H
#define KERNEL_STDIO_H

#include "stdio.h"

//! Size of the stream buffer
#define BUFSIZ 1024 * 32

//! Value returned when internal stream position has reached the end of file
#define EOF -1

//! Maximum length of file names
#define FILENAME_MAX 255

//! Potential limit of simultaneous open streams
#define FOPEN_MAX INT32_MAX

//! Minimum length for temporary file name
#define L_tmpnam 0

//! Maximum number of temporary files
#define TMP_MAX 0

//! Seek-Set mode (position is absolute)
#define SEEK_SET 0

//! Seek-Current mode (relative to the current position)
#define SEEK_CUR 1

//! Seek-End mode (relative to the end of the file)
#define SEEK_END 2

//! No Buffering mode
#define _IONBF 0

//! Line Buffering mode
#define _IOLBF 1

//! Full Buffering mode
#define _IOFBF 2

#include <stdint.h>
#include <stdarg.h>
#include "stdlib.h"

#ifdef __cplusplus
extern "C" {
#endif

int kernel_setvbuf(FILE *stream, char *buffer, int mode, size_t size);
int kernel_sprintf(char *str, const char *format, ...);
int kernel_vfprintf(FILE *stream, const char *format, va_list arg);
int kernel_fputc(int character, FILE *stream);
size_t kernel_fwrite(const void *ptr, size_t size, size_t count, FILE *stream);
int kernel_fflush(FILE *stream);

FILE *__kernel_stdio_create_stream();

#ifdef __cplusplus
}
#endif

#endif
