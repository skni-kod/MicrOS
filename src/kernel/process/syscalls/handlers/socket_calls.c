#include <process/syscalls/handlers/socket_calls.h>

void syscall_socket(interrupt_state *state)
{
    state->registers.eax = k_socket(state->registers.ebx, state->registers.ecx, state->registers.edx);
    return;
}

void syscall_socket_bind(interrupt_state *state)
{
    state->registers.eax = k_bind(state->registers.ebx, state->registers.ecx, state->registers.edx);
    return;
}

void syscall_socket_recv(interrupt_state *state)
{
    struct recv_params *params = state->registers.ebx;
    state->registers.eax = k_recv(
        params->s,
        params->buf,
        params->len,
        params->flags);
    return;
}

void syscall_socket_recvfrom(interrupt_state *state)
{
    struct recv_params *params = state->registers.ebx;
    state->registers.eax = k_recvfrom(
        params->s,
        params->buf,
        params->len,
        params->flags,
        params->from,
        params->fromlen);
    return;
}

void syscall_socket_send(interrupt_state *state)
{
    struct recv_params *params = state->registers.ebx;
    state->registers.eax = k_send(
        params->s,
        params->buf,
        params->len,
        params->flags);
    return;
}

void syscall_socket_sendto(interrupt_state *state)
{
    struct recv_params *params = state->registers.ebx;
    state->registers.eax = k_sendto(
        params->s,
        params->buf,
        params->len,
        params->flags,
        params->from,
        params->fromlen);
    return;
}

void syscall_socket_listen(interrupt_state *state)
{
    state->registers.eax = k_listen(state->registers.ebx, state->registers.ecx);
    return;
}

void syscall_socket_accept(interrupt_state *state)
{
    state->registers.eax = k_accept(state->registers.ebx, state->registers.ecx, state->registers.edx);
    return;
}
