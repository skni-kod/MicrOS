#include "../stdio.h"

void setbuf(FILE *stream, char *buffer)
{
    if (buffer == NULL)
    {
        stream->buffering_mode = file_buffering_mode_none;
    }
    else
    {
        free(stream->buffer);
        stream->buffer = buffer;
    }
}