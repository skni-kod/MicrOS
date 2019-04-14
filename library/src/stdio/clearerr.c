#include "../stdio.h"

void clearerr(FILE *stream)
{
    stream->error = 0;
}