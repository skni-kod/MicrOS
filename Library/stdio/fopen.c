#include "../stdio.h"

extern FILE *streams_set_stream_as_file(const char *filename, const char *mode, FILE *stream);

FILE *fopen(const char *filename, const char *mode)
{
    FILE *stream = __stdio_create_stream();
    if (!streams_set_stream_as_file(filename, mode, stream))
    {
        free(stream);
        return NULL;
    }

    return stream;
}