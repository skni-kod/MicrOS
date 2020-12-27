#ifndef SERIAL_CALLS_H
#define SERIAL_CALLS_H

#include "cpu/idt/state/interrupt_state.h"
#include "drivers/serial/serial.h"

void syscall_serial_init(interrupt_state *state);
void syscall_serial_is_busy(interrupt_state *state);
void syscall_serial_is_queue_empty(interrupt_state *state);
void syscall_serial_send(interrupt_state *state);
void syscall_serial_send_string(interrupt_state *state);
void syscall_serial_receive(interrupt_state *state);

#endif