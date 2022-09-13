#ifndef V8086_ARITHMETIC_OPERATIONS_H
#define V8086_ARITHMETIC_OPERATIONS_H

#include <stdbool.h>

int16_t perform_adding(v8086* machine, void* dest, void* source, uint8_t width, uint32_t carry);
int16_t perform_subtracting(v8086* machine, void* dest, void* source, uint8_t width, uint32_t carry);
int16_t perform_or(v8086* machine, void* dest, void* source, uint8_t width, uint32_t carry);
int16_t perform_and(v8086* machine, void* dest, void* source, uint8_t width, uint32_t carry);
int16_t perform_xor(v8086* machine, void* dest, void* source, uint8_t width, uint32_t carry);
int16_t perform_cmp(v8086* machine, void* dest, void* source, uint8_t width, uint32_t carry);
int16_t perform_ror(v8086* machine, void* dest, uint8_t arg, uint8_t width);
int16_t perform_rol(v8086* machine, void* dest, uint8_t arg, uint8_t width);
int16_t perform_rcl(v8086* machine, void* dest, uint8_t arg, uint8_t width);
int16_t perform_rcr(v8086* machine, void* dest, uint8_t arg, uint8_t width);
int16_t perform_shl(v8086* machine, void* dest, uint8_t arg, uint8_t width);
int16_t perform_shr(v8086* machine, void* dest, uint8_t arg, uint8_t width);
int16_t perform_sar(v8086* machine, void* dest, uint8_t arg, uint8_t width);
int16_t perform_neg(v8086* machine, void* source, uint8_t width);
int16_t perform_multiplication(v8086* machine, void* source, uint8_t signed_mul, uint8_t width);
int16_t perform_multiplication_3_byte(v8086* machine, void* dest, void* source, void* imm, uint8_t signed_mul, uint8_t width, uint8_t second_width);
int16_t perform_division(v8086* machine, void* source, uint8_t signed_div, uint8_t width);
int16_t perform_test(v8086* machine, void* source, void* dest, uint8_t width);
int16_t perform_inc(v8086* machine, void* dest, uint8_t width);
int16_t perform_dec(v8086* machine, void* dest, uint8_t width);

int16_t perform_artihmetic_or_logical_instruction(v8086* machine, uint8_t recalculated_opcode, uint32_t carry, int16_t (*operation)(v8086*, void*, void*, uint8_t, uint32_t));
int16_t perform_arithmetic_or_logical_instruction_group(v8086* machine, uint8_t recalculated_opcode, uint8_t mod_rm, uint32_t carry, int16_t (*operation)(v8086*, void*, void*, uint8_t, uint32_t));
int16_t perform_inc_dec(v8086* machine, uint8_t opcode, bool dec);
int16_t execute_test(v8086* machine, uint8_t opcode);
int16_t execute_test_immediate(v8086* machine, uint8_t opcode);
int16_t execute_group_2(v8086* machine, uint8_t opcode);
int16_t execute_group_3(v8086* machine, uint8_t opcode);
int16_t execute_double_shift(v8086* machine, uint8_t width, uint8_t right_shift, uint8_t use_cl);

#endif //V8086_ARITHMETIC_OPERATIONS_H
