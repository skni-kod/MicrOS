#include "../stdio.h"
#include "../errno.h"

size_t fwrite(const void *ptr, size_t size, size_t count, FILE *stream)
{
    //Before we do anything we should check if file is in writtable mode and we even have to do anything.
    uint32_t w;
    uint32_t total_size = size * count;
    uint32_t wlen = 0;
    char* p;
    if(total_size == 0) return 0;

    if(stream->mode == file_mode_read)
    {
        stream->error = EBADF;
        return EOF;
    }

    p = ptr;

#define	MIN(a, b) ((a) < (b) ? (a) : (b))

    switch (stream->buffering_mode)
    {
    case file_buffering_mode_none:
    {
        //we shouldn't have buffer in this mode, so we should write up to BUFSIZ at once into the file, repeat as long as we have remaining data.
        do
        {
            //Maybe consider some error checking here
            w = (*stream->write)(stream, p, MIN(total_size, BUFSIZ));
            //if(w <= 0) DoSomethingFancy
            p += w;
            //total_size -= w;
            stream->pos += w;
            stream->size += w;
            wlen += w;
        } while ((total_size -= w) != 0);
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
        wlen = total_size;
        break;
    }

    //WRONG
    case file_buffering_mode_full:
    {
        uint32_t offset = 0;
        

        memcpy(stream->buffer + (stream->pos - stream->base), ptr, total_size);
        stream->pos += total_size;
        stream->size += total_size;

        wlen = total_size;

        break;
    }
    }

    return wlen;
}