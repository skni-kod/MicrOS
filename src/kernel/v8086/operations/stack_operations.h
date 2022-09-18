#ifndef MICROS_STACK_OPERATIONS_H
#define MICROS_STACK_OPERATIONS_H

#include "../v8086.h"

int16_t push_gpr(v8086* machine, uint8_t opcode);
int16_t pop_gpr(v8086* machine, uint8_t opcode);
int16_t pop_rm(v8086* machine);
int16_t push_all(v8086* machine);
int16_t pop_all(v8086* machine);
int16_t push_immediate(v8086* machine, uint8_t width);
int16_t enter(v8086* machine, uint8_t width);
int16_t leave(v8086* machine, uint8_t width);


#endif //MICROS_STACK_OPERATIONS_H
