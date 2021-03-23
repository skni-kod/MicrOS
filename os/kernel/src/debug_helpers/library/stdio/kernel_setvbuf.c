#include "../../../memory/heap/heap.h"
#include "stdio.h"

int kernel_setvbuf(FILE *stream, char *buffer, int mode, size_t size)
{
    if (mode < 0 || mode > 2)
    {
        return -1;
    }

    heap_kernel_dealloc(stream->buffer);

    if (buffer == NULL)
    {
        buffer = heap_kernel_alloc(size, 0);
    }
    else
    {
        stream->buffer = buffer;
    }

    stream->buffering_mode = mode;
    return 0;
}