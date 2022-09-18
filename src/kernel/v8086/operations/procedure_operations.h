#ifndef MICROS_PROCEDURE_OPERATIONS_H
#define MICROS_PROCEDURE_OPERATIONS_H

#include <stdint.h>
#include <v8086/v8086.h>

int16_t near_relative_call(v8086* machine);
int16_t far_call(v8086* machine);

int16_t near_ret(v8086* machine);
int16_t near_ret_imm(v8086* machine);
int16_t far_ret(v8086* machine);
int16_t far_ret_imm(v8086* machine);
int16_t perform_interrupt(v8086* machine, uint8_t interrupt_number);
int16_t perform_iret(v8086* machine);

#endif //MICROS_PROCEDURE_OPERATIONS_H
