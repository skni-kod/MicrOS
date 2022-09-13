#ifndef MICROS_MISC_OPERATIONS_H
#define MICROS_MISC_OPERATIONS_H

#include <v8086/v8086.h>

int16_t perform_group_4(v8086* machine);
int16_t perform_group_5(v8086* machine);
int16_t setting_and_clearing_flags(v8086* machine, uint8_t opcode);
int16_t check_bounds(v8086* machine);
int16_t set_byte(v8086* machine, uint8_t opcode);

#endif //MICROS_MISC_OPERATIONS_H
