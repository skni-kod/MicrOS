#include "../string.h"

size_t strxfrm(char *destination, const char *source, size_t num)
{
    // NOTE: We use strcpy instead of something which considers locale settings. Will be changed later.
    strcpy(destination, source);
    return strlen(destination);
}