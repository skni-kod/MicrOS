#ifndef MICROS_INTERRUPTS_H
#define MICROS_INTERRUPTS_H

#include <stdint.h>

uint32_t micros_interrupt_0a(uint32_t function_number);
uint32_t micros_interrupt_1a(uint32_t function_number, uint32_t a);
uint32_t micros_interrupt_2a(uint32_t function_number, uint32_t a, uint32_t b);
uint32_t micros_interrupt_3a(uint32_t function_number, uint32_t a, uint32_t b, uint32_t c);
uint32_t micros_interrupt_4a(uint32_t function_number, uint32_t a, uint32_t b, uint32_t c, uint32_t d);
uint32_t micros_interrupt_5a(uint32_t function_number, uint32_t a, uint32_t b, uint32_t c, uint32_t d, uint32_t e);

#endif