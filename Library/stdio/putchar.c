#include "../stdio.h"

int putchar(int character)
{
    return fputc(character, stdout);
}