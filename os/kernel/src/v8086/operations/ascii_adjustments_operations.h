#ifndef MICROS_ASCII_ADJUSTMENTS_OPERATIONS_H
#define MICROS_ASCII_ADJUSTMENTS_OPERATIONS_H

int16_t adjust_after_add_sub(v8086* machine, bool sub);
int16_t adjust_after_add_sub_packed(v8086* machine, bool sub);
int16_t adjust_after_mul_div(v8086* machine, bool div);

#endif //MICROS_ASCII_ADJUSTMENTS_OPERATIONS_H
