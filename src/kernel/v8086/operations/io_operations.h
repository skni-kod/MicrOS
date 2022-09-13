#ifndef MICROS_IO_OPERATIONS_H
#define MICROS_IO_OPERATIONS_H

#include <stdint.h>
#include <v8086/v8086.h>

int16_t perform_in_imm(v8086* machine, uint8_t width);
int16_t perform_in_dx(v8086* machine, uint8_t width);
int16_t perform_out_imm(v8086* machine, uint8_t width);
int16_t perform_out_dx(v8086* machine, uint8_t width);
int16_t perform_ins_dx(v8086* machine, uint8_t width);
int16_t perform_outs_dx(v8086* machine, uint8_t width);

#endif //MICROS_IO_OPERATIONS_H
