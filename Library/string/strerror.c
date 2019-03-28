#include "../string.h"

// TODO: add valid error codes
char *error_messages[] =
{
    ""
};

char *strerror(int errnum)
{
    return error_messages[errnum];
}