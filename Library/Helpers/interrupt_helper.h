#ifndef INTERRUPT_HELPER_H
#define INTERRUPT_HELPER_H

#include <stdint.h>

uint32_t call_interrupt_0a(uint32_t function_number);
uint32_t call_interrupt_1a(uint32_t function_number, uint32_t a);
uint32_t call_interrupt_2a(uint32_t function_number, uint32_t a, uint32_t b);
uint32_t call_interrupt_3a(uint32_t function_number, uint32_t a, uint32_t b, uint32_t c);

#endif