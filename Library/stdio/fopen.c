#include "../stdio.h"

FILE *fopen(const char *filename, const char *mode)
{
    FILE *stream = streams_create_stream();
    streams_set_stream_as_file(filename, mode, stream);

    return stream;
}