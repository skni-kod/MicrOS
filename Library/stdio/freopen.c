#include "../stdio.h"

extern FILE *streams_set_stream_as_file(const char *, const char *, FILE *);

FILE *freopen(const char *filename, const char *mode, FILE *stream)
{
    if (!streams_set_stream_as_file(filename == NULL ? stream->filename : filename, mode, stream))
    {
        return NULL;
    }

    return stream;
}