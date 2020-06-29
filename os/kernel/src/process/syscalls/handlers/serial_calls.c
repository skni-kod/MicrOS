#include "serial_calls.h"

void syscall_serial_init(interrupt_state *state)
{
    serial_init(state->registers.ebx, state->registers.ecx, state->registers.edx, state->registers.esi, state->registers.edi);
}

void syscall_serial_is_busy(interrupt_state *state)
{
    state->registers.eax = serial_is_busy(state->registers.ebx);
}

void syscall_serial_is_queue_empty(interrupt_state *state)
{
    state->registers.eax = serial_is_queue_empty(state->registers.ebx);
}

void syscall_serial_send(interrupt_state *state)
{
    serial_send(state->registers.ebx, state->registers.ecx);
}

void syscall_serial_send_string(interrupt_state *state)
{
    serial_send_string(state->registers.ebx, state->registers.ecx);
}

void syscall_serial_receive(interrupt_state *state)
{
    state->registers.eax = serial_receive(state->registers.ebx);
}