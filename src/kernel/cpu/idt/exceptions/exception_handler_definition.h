#ifndef EXCEPTION_HANDLER_DEFINITION_H
#define EXCEPTION_HANDLER_DEFINITION_H

#include <stdint.h>

typedef struct exception_handler_definition
{
    uint8_t exception_number;
    void (*handler)(exception_state *state);
} exception_handler_definition;

#endif