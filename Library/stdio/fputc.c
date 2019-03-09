#include "../stdio.h"

int fputc(int character, FILE *stream)
{
    fwrite(&character, 1, 1, stream);
    return character;
}