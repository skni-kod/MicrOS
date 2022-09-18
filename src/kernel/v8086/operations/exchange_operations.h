#ifndef MICROS_EXCHANGE_OPERATIONS_H
#define MICROS_EXCHANGE_OPERATIONS_H

#include <v8086/v8086.h>

int16_t perform_exchange_rm(v8086* machine, uint8_t opcode);
int16_t perform_exchange_ax_register(v8086* machine, uint8_t opcode);

#endif //MICROS_EXCHANGE_OPERATIONS_H
