#include "../kernel_stdio.h"

int kernel_fputc(int character, FILE *stream)
{
    if (kernel_fwrite(&character, 1, 1, stream) != 1)
    {
        stream->error = 1;
        return EOF;
    }

    return character;
}