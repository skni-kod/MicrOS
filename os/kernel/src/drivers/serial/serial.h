#ifndef SERIAL_H
#define SERIAL_H

#include <stdbool.h>
#include "assembly/io.h"

#define COM1_PORT 0x3F8
#define COM2_PORT 0x2F8
#define COM3_PORT 0x3E8
#define COM4_PORT 0x2E8

#define DATA_REGISTER 0
#define INTERRUPT_ENABLE_REGISTER 1
#define DIVISOR_LOW_REGISTER 0
#define DIVISOR_HIGH_REGISTER 1
#define INTERRUPT_FIFO_CONTROL_REGISTER 2
#define LINE_CONTROL_REGISTER 3
#define MODEM_CONTROL_REGISTER 4
#define LINE_STATUS_REGISTER 5
#define MODEM_STATUS_REGISTER 6
#define SCRATCH_REGISTER 7

#define PARITY_NONE 0
#define PARITY_ODD 1
#define PARITY_EVEN 3
#define PARITY_MARK 5
#define PARITY_SPACE 7

void serial_init(unsigned int port, unsigned int baud_rate, unsigned int data_bits, unsigned int stop_bits, unsigned int parity);
bool serial_is_busy(unsigned int port);
bool serial_is_queue_empty(unsigned int port);
void serial_send(unsigned int port, char c);
void serial_send_string(unsigned int port, char *str);
char serial_receive(unsigned int port);

#endif