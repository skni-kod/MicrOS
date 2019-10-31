#include "../stdio.h"

int ungetc(int character, FILE *stream)
{
    if (stream->pos == 0)
    {
        return EOF;
    }

    return stream->buffer[stream->pos-- - stream->base - 1];
}