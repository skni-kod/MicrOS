#ifndef SOCKET_CALLS_H
#define SOCKET_CALLS_H

#include <process/socket/socket.h>
#include <micros/sys/micros_socket.h>
#include <cpu/idt/state/interrupt_state.h>

void syscall_socket(interrupt_state *state);

void syscall_socket_bind(interrupt_state *state);

void syscall_socket_recv(interrupt_state *state);

void syscall_socket_recvfrom(interrupt_state *state);

void syscall_socket_send(interrupt_state *state);

void syscall_socket_sendto(interrupt_state *state);

void syscall_socket_listen(interrupt_state *state);

void syscall_socket_accept(interrupt_state *state);

void syscall_socket_connect(interrupt_state *state);

#endif