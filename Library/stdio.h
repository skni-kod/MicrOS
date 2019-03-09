#ifndef STDIO_H
#define STDIO_H

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
#include "stdlib.h"
#include "Streams/file.h"

typedef uint32_t size_t;
typedef uint32_t fpos_t;

FILE *stdin;
FILE *stdout;
FILE *stderr;

FILE *fopen(const char *filename, const char *mode);
FILE *freopen(const char *filename, const char *mode, FILE *stream);
int fclose(FILE *stream);
int fflush(FILE *stream);
void setbuf(FILE *stream, char *buffer);
int setvbuf(FILE *stream, char *buffer, int mode, size_t size);

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

size_t fread(void *ptr, size_t size, size_t count, FILE *stream);
size_t fwrite(const void *ptr, size_t size, size_t count, FILE *stream);

int fgetpos(FILE *stream, fpos_t *pos);
int fseek(FILE *stream, long int offset, int origin);
int fsetpos(FILE *stream, const fpos_t *pos);
long int ftell(FILE *stream);
void rewind(FILE *stream);

void clearerr(FILE *stream);
int feof(FILE *stream);
int ferror(FILE *stream);

FILE *streams_create_stream();

#endif