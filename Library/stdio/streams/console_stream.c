#include "console_stream.h"

FILE *streams_set_stream_as_console(FILE *stream)
{
    stream->buffering_mode = file_buffering_mode_none;
    stream->fetch = streams_console_fetch;
    stream->flush = streams_console_flush;

    return stream;
}

int streams_console_fetch(FILE *stream)
{
    // Console isn't a valid device to read anything
    return 0;
}

void streams_console_flush(FILE *stream)
{
    stream->buffer[stream->pos] = 0;
    micros_console_print_string(stream->buffer);
}