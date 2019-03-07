#ifndef STDIO_H
#define STDIO_H

#define BUFSIZ 1024

#include <stdint.h>
#include "micros.h"

typedef struct FILE
{
    char* buffer;
    uint32_t pos;
    uint32_t size;
    
    void (*fetch)(struct FILE* file);
    void (*flush)(struct FILE* file);
} FILE;

FILE *stdin;
FILE *stdout;
FILE *stderr;

int fputs(const char* str, FILE* stream);
int fflush(FILE* stream);

FILE* streams_create_stream();
FILE* streams_set_stream_as_console(FILE* stream);
void streams_console_fetch(FILE* stream);
void streams_console_flush(FILE* stream);

#endif