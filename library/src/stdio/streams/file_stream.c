#include "file_stream.h"

FILE *streams_set_stream_as_file(const char *filename, const char *mode, FILE *stream)
{
    micros_filesystem_file_info info;
    file_mode parsed_mode = __stdio_get_file_mode(mode);

    if (!micros_filesystem_file_exists((char *)filename))
    {
        if (parsed_mode == file_mode_read || parsed_mode == file_mode_read_and_update)
        {
            return 0;
        }
        else
        {
            if (!micros_filesystem_create_file((char *)filename))
            {
                return NULL;
            }
        }
    }

    micros_filesystem_get_file_info((char *)filename, &info);

    stream->pos = 0;
    stream->mode = parsed_mode;
    stream->limit = info.size;
    stream->buffering_mode = file_buffering_mode_full;
    stream->fetch = streams_file_fetch;
    stream->flush = streams_file_flush;

    memcpy(stream->filename, filename, strlen(filename)+1);
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
    if (stream->mode == file_mode_write || stream->mode == file_mode_write_and_update)
    {
        micros_filesystem_save_to_file(stream->filename, stream->buffer, stream->size);
    }
    else if (stream->mode == file_mode_append || stream->mode == file_mode_append_and_update)
    {
        micros_filesystem_append_to_file(stream->filename, stream->buffer, stream->size);
    }
}