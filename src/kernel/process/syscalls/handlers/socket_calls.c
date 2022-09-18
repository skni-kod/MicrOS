#include <process/syscalls/handlers/socket_calls.h>
#include <process/socket/socket.h>

void syscall_socket(interrupt_state *state)
{
    state->registers.eax = k_socket(state->registers.ebx, state->registers.ecx, state->registers.edx, state->registers.esi );
    return;
}

void syscall_recvfrom(interrupt_state *state)
{
    state->registers.eax = k_recvfrom(state->registers.ebx, state->registers.ecx, state->registers.edx);
    return;
}