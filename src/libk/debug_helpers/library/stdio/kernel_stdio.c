#include "memory/heap/heap.h"
#include <stdio.h>

FILE *__kernel_stdio_create_stream()
{
    FILE *stream = heap_kernel_alloc(sizeof(FILE), 0);
    stream->buffer = heap_kernel_alloc(BUFSIZ, 0);
    stream->base = 0;
    stream->pos = 0;
    stream->size = 0;
    stream->limit = 0;
    stream->buffering_mode = file_buffering_mode_line;

    return stream;
}