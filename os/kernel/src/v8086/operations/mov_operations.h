#ifndef MICROS_MOV_OPERATIONS_H
#define MICROS_MOV_OPERATIONS_H

#include <v8086/v8086.h>

int16_t perform_mov_rm(v8086* machine, uint8_t opcode);
int16_t perform_mov_segment(v8086* machine, uint8_t opcode);
int16_t perform_lea(v8086* machine);

#endif //MICROS_MOV_OPERATIONS_H
