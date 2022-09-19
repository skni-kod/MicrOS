#include <process/syscalls/handlers/socket_calls.h>
#include <process/socket/socket.h>
#include <micros/sys/micros_socket.h>

void syscall_socket(interrupt_state *state)
{
    state->registers.eax = socket(state->registers.ebx, state->registers.ecx, state->registers.edx);
    return;
}

void syscall_socket_bind(interrupt_state *state)
{
    state->registers.eax = bind(state->registers.ebx, state->registers.ecx, state->registers.edx);
    return;
}

void syscall_socket_recvfrom(interrupt_state *state)
{
    struct recv_params *params = state->registers.ebx;
    state->registers.eax = recvfrom(
            params->s,
            params->buf,
            params->len,
            params->flags,
            params->from,
            params->fromlen
            );
    return;
}

void syscall_socket_sendto(interrupt_state *state)
{
    struct recv_params *params = state->registers.ebx;
    state->registers.eax = sendto(
        params->s,
        params->buf,
        params->len,
        params->flags,
        params->from,
        params->fromlen
    );
    return;
}