#ifndef ASSERT_H
#define ASSERT_H

#include "stdio.h"

// TODO: add fprintf in format "Assertion failed: expression, file filename, line line number"
#define STR(x) #x
#define assert(expression)                   \
    if (!(expression))                       \
    {                                        \
        fputs("Assertion failed: ", stderr); \
        abort();                             \
    }

#endif