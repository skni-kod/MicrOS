#ifndef SOCKET_CALLS_H
#define SOCKET_CALLS_H

#include <cpu/idt/state/interrupt_state.h>

void syscall_socket(interrupt_state *state);

void syscall_recvfrom(interrupt_state *state);


#endif