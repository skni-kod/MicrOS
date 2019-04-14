#ifndef EXCEPTION_DEFINITION_H
#define EXCEPTION_DEFINITION_H

#include <stdint.h>
#include <stdbool.h>

typedef struct exception_definition
{
    char description[32];
    uint8_t interrupt_number;
} exception_definition;

#endif