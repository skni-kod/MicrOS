#include "../stdio.h"

int fclose(FILE *stream)
{
    fflush(stream);
    free(stream);

    return 0;
}