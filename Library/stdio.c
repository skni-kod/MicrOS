#include "stdio.h"

int fclose(FILE * stream)
{
    fflush(stream);
    free(stream);
}

int fflush(FILE* stream)
{
    stream->flush(stream);
    stream->pos = 0;
}

int fputc(int character, FILE* stream)
{
    if(stream->pos + 1 > stream->size)
    {
        fflush(stream);
    }
    
    stream->buffer[stream->pos] = character;
    stream->pos++;
    
    return character;
}

int fputs(const char* str, FILE* stream)
{
    return fwrite(str, strlen(str), 1, stream);
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

size_t fwrite(const void* ptr, size_t size, size_t count, FILE* stream)
{
    uint32_t total_size = size * count;
    
    if(stream->pos + total_size > stream->size)
    {
        fflush(stream);
    }
    
    memcpy(stream->buffer + stream->pos, ptr, total_size);
    
    stream->pos += total_size;
    return 0;
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