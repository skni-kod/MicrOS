#include "../stdio.h"

size_t fread(void *ptr, size_t size, size_t count, FILE *stream)
{
    int c = fgetc(stream);
    int i = 0;
    int total_size = size * count;

    do
    {
        ((char *)ptr)[i++] = c;
    } while (i < total_size && (c = fgetc(stream), c != EOF));

    return i;
}