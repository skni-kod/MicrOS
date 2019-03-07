#include "stdio.h"

int fputs(const char* str, FILE* stream)
{
    uint32_t str_length = strlen(str);
    if(stream->size < stream->size + str_length)
    {
        memcpy(stream->buffer + stream->pos, str, str_length);
    }
}

int fflush(FILE* stream)
{
    stream->flush(stream);
    stream->pos = 0;
}

FILE* streams_create_stream()
{
    FILE* stream = malloc(sizeof(FILE));
    stream->buffer = malloc(BUFSIZ);
    stream->size = BUFSIZ;
    stream->pos = 0;
    
    return stream;
}

FILE* streams_set_stream_as_console(FILE* stream)
{
    stream->fetch = streams_console_fetch;
    stream->flush = streams_console_flush;
}

void streams_console_fetch(FILE* stream)
{
    // Console isn't a valid device to read anything
}

void streams_console_flush(FILE* stream)
{
    micros_console_print_string(stream->buffer);
}