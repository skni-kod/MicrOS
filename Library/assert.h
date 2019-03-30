#ifndef ASSERT_H
#define ASSERT_H

#include "stdio.h"

// TODO: add fprintf in format "Assertion failed: expression, file filename, line line number"
#define STR(x) #x
#define assert(expression)                                                      \
    if (!(expression))                                                          \
    {                                                                           \
        fprintf(stderr, "Assertion failed: %s at line %d", __FILE__, __LINE__); \
        abort();                                                                \
    }

#endif