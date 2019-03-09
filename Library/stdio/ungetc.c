#include "../stdio.h"

int ungetc(int character, FILE *stream)
{
    if (stream->pos > 0)
    {
        stream->pos++;
    }

    return stream->buffer[stream->pos - stream->base - 1];
}