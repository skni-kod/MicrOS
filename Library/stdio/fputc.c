#include "../stdio.h"

int fputc(int character, FILE *stream)
{
    if (fwrite(&character, 1, 1, stream) != 1)
    {
        stream->error = 1;
        return EOF;
    }

    return character;
}