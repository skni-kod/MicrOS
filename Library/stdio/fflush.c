#include "../stdio.h"

int fflush(FILE *stream)
{
    stream->flush(stream);
    stream->pos = 0;
    stream->size = 0;
    stream->base = 0;

    return 0;
}