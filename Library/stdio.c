#include "stdio.h"

FILE *fopen(const char *filename, const char *mode)
{
    FILE *stream = streams_create_stream();
    streams_set_stream_as_file(filename, mode, stream);
}

int fclose(FILE *stream)
{
    fflush(stream);
    free(stream);
}

int fflush(FILE *stream)
{
    stream->flush(stream);
    stream->pos = 0;
    stream->size = 0;
}

int fgetc(FILE *stream)
{
    if (stream->pos == stream->size)
    {
        if (stream->fetch(stream) == 0)
        {
            return EOF;
        }

        stream->pos = 0;
    }

    return stream->buffer[stream->pos++];
}

char *fgets(char *str, int num, FILE *stream)
{
    char c = fgetc(stream);
    int i = 0;

    do
    {
        str[i++] = c;
    } while ((c = fgetc(stream), c != '\n') && i < num);

    str[i] = 0;
    return str;
}

int fputc(int character, FILE *stream)
{
    fwrite(&character, 1, 1, stream);
    return character;
}

int fputs(const char *str, FILE *stream)
{
    return fwrite(str, strlen(str), 1, stream);
}

int getc(FILE *stream)
{
    return fgetc(stream);
}

int getchar()
{
    return fgetc(stdin);
}

char *gets(char *str)
{
    return fgets(str, INT32_MAX, stdin);
}

int putc(int character, FILE *stream)
{
    return fputc(character, stream);
}

int putchar(int character)
{
    return fputc(character, stdout);
}

int puts(const char *str)
{
    return fputs(str, stdout);
}

int ungetc(int character, FILE *stream)
{
    if (stream->pos > 0)
    {
        stream->pos++;
    }

    return stream->buffer[stream->pos - 1];
}

size_t fread(void *ptr, size_t size, size_t count, FILE *stream)
{
    char c = fgetc(stream);
    int i = 0;
    int total_size = size * count;

    do
    {
        ((char *)ptr)[i++] = c;
    } while (i < total_size && (c = fgetc(stream), c != EOF));

    return i;
}

size_t fwrite(const void *ptr, size_t size, size_t count, FILE *stream)
{
    uint32_t total_size = size * count;

    if (stream->pos + total_size > BUFSIZ)
    {
        fflush(stream);
    }

    switch (stream->buffering_mode)
    {
    case file_buffering_mode_none:
    {
        memcpy(stream->buffer + stream->pos, ptr, total_size);
        stream->pos += total_size;
        stream->size += total_size;

        fflush(stream);

        break;
    }

    case file_buffering_mode_line:
    {
        uint32_t origin = 0;
        for (int i = origin; i < total_size; i++)
        {
            char c = ((const char *)ptr)[i];
            if (c == '\n')
            {
                uint32_t bytes_to_copy = i - origin + 1;

                memcpy(stream->buffer + stream->pos, ptr + origin, bytes_to_copy);
                fflush(stream);

                origin = i + 1;
            }
        }

        if (origin != total_size)
        {
            uint32_t bytes_to_copy = total_size - origin - 1;

            memcpy(stream->buffer + stream->pos, ptr + origin, bytes_to_copy);
            stream->pos += bytes_to_copy;
            stream->size += bytes_to_copy;
        }

        break;
    }

    case file_buffering_mode_full:
    {
        memcpy(stream->buffer + stream->pos, ptr, total_size);
        stream->pos += total_size;
        stream->size += total_size;

        break;
    }
    }

    return total_size;
}

FILE *streams_create_stream()
{
    FILE *stream = malloc(sizeof(FILE));
    stream->buffer = malloc(BUFSIZ);
    stream->base = 0;
    stream->pos = 0;
    stream->size = 0;
    stream->limit = 0;
    stream->buffering_mode = file_buffering_mode_line;

    return stream;
}