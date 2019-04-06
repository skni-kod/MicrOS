#include "../string.h"

int strcoll(const char *str1, const char *str2)
{
    // NOTE: We use strcmp instead of something which considers locale settings. Will be changed later.
    return strcmp(str1, str2);
}