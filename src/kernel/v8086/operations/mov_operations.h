#ifndef MICROS_MOV_OPERATIONS_H
#define MICROS_MOV_OPERATIONS_H

#include <v8086/v8086.h>

int16_t perform_mov_rm(v8086* machine, uint8_t opcode);
int16_t perform_mov_segment(v8086* machine, uint8_t opcode);
int16_t perform_mov_moffset(v8086* machine, uint8_t opcode);
uint16_t perform_mov_gpr_imm(v8086* machine, uint8_t opcode);
uint16_t perform_mov_rm_imm(v8086* machine, uint8_t opcode);
int16_t perform_lea(v8086* machine);
int16_t convert_byte_to_word(v8086* machine);
int16_t convert_word_to_double(v8086* machine);
int16_t store_flags(v8086* machine);
int16_t load_flags(v8086* machine);
int16_t perform_load_far_pointer(v8086* machine, segment_register_select segment_op);
int16_t perform_xlat(v8086* machine);
int16_t perform_movzx(v8086* machine, uint8_t opcode);
int16_t perform_movsx(v8086* machine, uint8_t opcode);

#endif //MICROS_MOV_OPERATIONS_H
