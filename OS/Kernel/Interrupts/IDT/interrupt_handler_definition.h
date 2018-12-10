#ifndef INTERRUPT_HANDLER_DEFINITION_H
#define INTERRUPT_HANDLER_DEFINITION_H

#include <stdint.h>

typedef struct interrupt_handler_definition
{
    uint8_t interrupt_number;
    void (*handler)(interrupt_state* state);
} interrupt_handler_definition;

#endif