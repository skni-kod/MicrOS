#ifndef MICROS_JUMP_OPERATIONS_H
#define MICROS_JUMP_OPERATIONS_H

#include "../v8086.h"

int16_t jump_short_relative(v8086* machine);
int16_t jump_near_relative(v8086* machine);
int16_t jump_far(v8086* machine);
int16_t perform_loop_loopne(v8086* machine, uint8_t opcode);
int16_t jump_short_relative_on_condition(v8086* machine, uint8_t opcode);
int16_t jump_on_condition(v8086* machine, uint8_t opcode, uint8_t width);

#endif //MICROS_JUMP_OPERATIONS_H
