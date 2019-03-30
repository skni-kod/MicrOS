#ifndef ASSERT_H
#define ASSERT_H

#include "stdio.h"

#define assert(expression)                                                      \
    if (!(expression))                                                          \
    {                                                                           \
        fprintf(stderr, "Assertion failed: %s at line %d", __FILE__, __LINE__); \
        abort();                                                                \
    }

#endif