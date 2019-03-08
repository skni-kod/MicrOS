#include "file_stream.h"

FILE *streams_set_stream_as_file(const char *filename, const char *mode, FILE *stream)
{
    micros_filesystem_file_info info;
    if (!micros_filesystem_get_file_info(filename, &info))
    {
        return 0;
    }

    stream->buffering_mode = file_buffering_mode_none;
    stream->fetch = streams_file_fetch;
    stream->flush = streams_file_flush;

    memcpy(stream->path, filename, strlen(filename));
    return stream;
}

int streams_file_fetch(FILE *stream)
{
}

void streams_file_flush(FILE *stream)
{
}