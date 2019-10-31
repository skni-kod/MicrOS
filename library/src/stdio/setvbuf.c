#include "../stdio.h"

int setvbuf(FILE *stream, char *buffer, int mode, size_t size)
{
    if (mode < 0 || mode > 2)
    {
        return -1;
    }

    free(stream->buffer);

    if (buffer == NULL)
    {
        buffer = malloc(size);
    }
    else
    {
        stream->buffer = buffer;
    }

    stream->buffering_mode = mode;
    return 0;
}