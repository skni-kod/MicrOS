#ifndef MICROS_JUMP_OPERATIONS_H
#define MICROS_JUMP_OPERATIONS_H

#include "../v8086.h"

int16_t jump_short_relative(v8086* machine);
int16_t jump_short_relative_on_condition(v8086* machine, uint8_t opcode);

#endif //MICROS_JUMP_OPERATIONS_H
