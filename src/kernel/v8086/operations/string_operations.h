#include <stdint.h>
#include <v8086/v8086.h>

#ifndef MICROS_STRING_OPERATIONS_H
#define MICROS_STRING_OPERATIONS_H

uint16_t perform_movs(v8086* machine, uint8_t width);
uint16_t perform_stos(v8086* machine, uint8_t width);
uint16_t perform_cmps(v8086* machine, uint8_t width);
uint16_t perform_lods(v8086* machine, uint8_t width);
uint16_t perform_scas(v8086* machine, uint8_t width);


#endif //MICROS_STRING_OPERATIONS_H
