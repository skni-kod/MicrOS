#ifndef STDIO_H
#define STDIO_H
#include <stdint.h>

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

#endif