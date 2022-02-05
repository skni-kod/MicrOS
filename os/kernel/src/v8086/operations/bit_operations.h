#ifndef MICROS_BIT_OPERATIONS_H
#define MICROS_BIT_OPERATIONS_H

#include "v8086/v8086.h"

int16_t bit_test(v8086* machine, uint8_t width);
int16_t bit_test_set(v8086* machine, uint8_t width);
int16_t bit_test_reset(v8086* machine, uint8_t width);
int16_t bit_test_complement(v8086* machine, uint8_t width);
int16_t perform_group_8(v8086* machine, uint8_t width);
int16_t bit_scan_forward(v8086* machine, uint8_t width);
int16_t bit_scan_backward(v8086* machine, uint8_t width);

#endif //MICROS_BIT_OPERATIONS_H
