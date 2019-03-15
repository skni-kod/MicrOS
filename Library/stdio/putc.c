#include "../stdio.h"

int putc(int character, FILE *stream)
{
    return fputc(character, stream);
}