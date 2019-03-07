#include "stdio.h"

int fputc(int character, FILE* stream)
{
    streams_expand_buffer_to_size(stream, stream->pos + 1);
    stream->buffer[stream->pos] = character;
    stream->pos++;
    
    return character;
}

int fputs(const char* str, FILE* stream)
{
    uint32_t str_length = strlen(str);
    
    streams_expand_buffer_to_size(stream, stream->pos + str_length);
    memcpy(stream->buffer + stream->pos, str, str_length);
    
    stream->pos += str_length;
    return 0;
}

int putc(int character, FILE* stream)
{
    return fputc(character, stream);
}

int putchar(int character)
{
    return fputc(character, stdout);
}

int puts(const char* str)
{
    return fputs(str, stdout);
}

int ungetc(int character, FILE* stream)
{
    if(stream->pos > 0)
    {
        stream->pos++;
    }
    
    return stream->buffer[stream->pos - 1];
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

void streams_expand_buffer_to_size(FILE* stream, uint32_t needed_size)
{
    while(stream->size <= stream->pos + needed_size)
    {
        stream->size *= 2;
        stream->buffer = realloc(stream->buffer, stream->size);
    }
}