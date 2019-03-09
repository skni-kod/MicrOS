#include "../stdio.h"

int fseek(FILE *stream, long int offset, int origin)
{
    uint32_t backup_pos = stream->pos;

    switch (origin)
    {
    case SEEK_SET:
    {
        stream->pos = offset;
        break;
    }

    case SEEK_CUR:
    {
        stream->pos += offset;
        break;
    }

    case SEEK_END:
    {
        stream->pos = stream->limit + offset;
        break;
    }
    }

    if (stream->pos > stream->limit)
    {
        stream->pos = backup_pos;
        return -1;
    }

    stream->base = stream->pos;
    stream->size = stream->pos;

    return 0;
}