#include "file_stream.h"

FILE *streams_set_stream_as_file(const char *filename, const char *mode, FILE *stream)
{
    micros_filesystem_file_info info;
    if (!micros_filesystem_get_file_info((char *)filename, &info))
    {
        return 0;
    }

    stream->limit = info.size;
    stream->buffering_mode = file_buffering_mode_none;
    stream->fetch = streams_file_fetch;
    stream->flush = streams_file_flush;

    memcpy(stream->filename, filename, strlen(filename));
    return stream;
}

int streams_file_fetch(FILE *stream)
{
    uint32_t bytes_to_read = stream->pos + CHUNK_SIZE > stream->limit ? stream->limit - stream->pos : CHUNK_SIZE;

    if (bytes_to_read == 0)
    {
        return 0;
    }

    micros_filesystem_read_file(stream->filename, (uint8_t *)stream->buffer, stream->pos, CHUNK_SIZE);

    stream->size = stream->pos + bytes_to_read;
    stream->base = stream->pos;

    return bytes_to_read;
}

void streams_file_flush(FILE *stream)
{
    // Currently not supported
}