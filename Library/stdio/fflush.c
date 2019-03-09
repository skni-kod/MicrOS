#include "../stdio.h"

int fflush(FILE *stream)
{
    if (stream == NULL)
    {
        return fflush(stdin) | fflush(stdout) | fflush(stderr);
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