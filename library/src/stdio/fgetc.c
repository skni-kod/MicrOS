#include "../stdio.h"

int fgetc(FILE *stream)
{
    if (stream->pos == stream->size)
    {
        if (stream->fetch(stream) == 0)
        {
            return EOF;
        }
    }

    return (unsigned char)stream->buffer[stream->pos++ - stream->base];
}