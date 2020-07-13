#ifndef MICROS_STACK_OPERATIONS_H
#define MICROS_STACK_OPERATIONS_H

#include "../v8086.h"

uint16_t push_gpr(v8086* machine, uint8_t opcode);
uint16_t pop_gpr(v8086* machine, uint8_t opcode);
uint16_t pop_rm(v8086* machine);
uint16_t push_all(v8086* machine);
uint16_t pop_all(v8086* machine);


#endif //MICROS_STACK_OPERATIONS_H
