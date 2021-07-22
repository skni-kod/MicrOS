#include "../kernel_stdio.h"
#include "stdio.h"

int kernel_fflush(FILE *stream)
{
    if (stream == NULL)
    {
        return kernel_fflush(stdin) | kernel_fflush(stdout) | kernel_fflush(stderr);
    }
    else
    {
        stream->flush(stream);
        stream->pos = 0;
        stream->size = 0;
        stream->base = 0;
    }

    return 0;
}