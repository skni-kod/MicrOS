#ifndef IO_H
#define IO_H

#include <stdint.h>

uint8_t io_in_byte(uint16_t port);
void io_out_byte(uint16_t port, uint8_t val);
uint16_t io_in_word(uint16_t port);
void io_out_word(uint16_t port, uint16_t val);
uint32_t io_in_long(uint16_t port);
void io_out_long(uint16_t port, uint32_t val);
void io_enable_interrupts();
void io_disable_interrupts();
void io_wait();

#endif