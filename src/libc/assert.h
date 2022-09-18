#ifndef ASSERT_H
#define ASSERT_H

#include "stdio.h"

//! Check if value is upper alphanumeric
/*!  
    If the argument expression of this macro with functional form compares equal to zero (i.e., the expression is false), a message is written to the standard error device and abort is called, terminating the program execution.
    \param expression Expression to be evaluated. If this expression evaluates to 0, this causes an assertion failure that terminates the program..
*/
#define assert(expression)                                                                          \
    if (!(expression))                                                                              \
    {                                                                                               \
        fprintf(stderr, "Assertion failed (%s): %s at line %d", #expression, __FILE__, __LINE__);   \
        abort();                                                                                    \
    }

#endif