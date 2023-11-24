#include "file_stream.h"
#include "../../string.h"

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

    strcpy(stream->filename, filename);
    stream->pos = 0;
    stream->mode = parsed_mode;
    stream->limit = info.size;
    stream->buffering_mode = file_buffering_mode_full;
    stream->fetch = streams_file_fetch;
    stream->flush = streams_file_flush;
    stream->write = streams_file_write;

    //Reset file stream and save to disk when opening. This way we have proper w/w+ modes support without problems with flush appending.
    if(parsed_mode == file_mode_write || parsed_mode == file_mode_write_and_update)
        micros_filesystem_save_to_file(stream->filename, stream->buffer, stream->size);

    return stream;
}

int streams_file_fetch(FILE *stream)
{
    uint32_t bytes_to_read = stream->pos + BUFSIZ > stream->limit ? stream->limit - stream->pos : BUFSIZ;

    if (bytes_to_read == 0)
    {
        return 0;
    }

    micros_filesystem_read_file(stream->filename, (uint8_t *)stream->buffer, stream->pos, BUFSIZ);

    stream->size = stream->pos + bytes_to_read;
    stream->base = stream->pos;

    return bytes_to_read;
}

void streams_file_flush(FILE *stream)
{
    //Since we took care of file clearing after opening it we can now just append to it all the time. SHOULD FIX STREAMS!
    if(stream->mode != file_mode_read && stream->mode != file_mode_read_and_update && stream->buffering_mode == file_buffering_mode_full)
        micros_filesystem_append_to_file(stream->filename, stream->buffer, stream->size);
    // if (stream->mode == file_mode_write || stream->mode == file_mode_write_and_update)
    // {
    //     micros_filesystem_save_to_file(stream->filename, stream->buffer, stream->size);
    // }
    // else if (stream->mode == file_mode_append || stream->mode == file_mode_append_and_update)
    // {
    //     micros_filesystem_append_to_file(stream->filename, stream->buffer, stream->size);
    // }
}

int streams_file_write(FILE* stream, const char* ptr, int count)
{
    if(micros_filesystem_append_to_file(stream->filename, ptr, count)) return count;
    else return 0;
}