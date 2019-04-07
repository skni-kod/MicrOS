#include "../stdio.h"

size_t fwrite(const void *ptr, size_t size, size_t count, FILE *stream)
{
    uint32_t total_size = size * count;

    if (stream->pos + total_size > BUFSIZ)
    {
        fflush(stream);
    }

    switch (stream->buffering_mode)
    {
    case file_buffering_mode_none:
    {
        memcpy(stream->buffer + (stream->pos - stream->base), ptr, total_size);
        stream->pos += total_size;
        stream->size += total_size;

        fflush(stream);

        break;
    }

    case file_buffering_mode_line:
    {
        uint32_t origin = 0;
        for (uint32_t i = origin; i < total_size; i++)
        {
            char c = ((const char *)ptr)[i];
            if (c == '\n')
            {
                uint32_t bytes_to_copy = i - origin + 1;

                memcpy(stream->buffer + (stream->pos - stream->base), ptr + origin, bytes_to_copy);
                fflush(stream);

                origin = i + 1;
            }
        }

        if (origin != total_size)
        {
            uint32_t bytes_to_copy = total_size - origin - 1;

            memcpy(stream->buffer + (stream->pos - stream->base), ptr + origin, bytes_to_copy);
            stream->pos += bytes_to_copy;
            stream->size += bytes_to_copy;
        }

        break;
    }

    case file_buffering_mode_full:
    {
        memcpy(stream->buffer + (stream->pos - stream->base), ptr, total_size);
        stream->pos += total_size;
        stream->size += total_size;

        break;
    }
    }

    return total_size;
}