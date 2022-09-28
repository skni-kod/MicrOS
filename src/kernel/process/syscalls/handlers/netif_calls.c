#include <process/syscalls/handlers/netif_calls.h>

void syscall_netif_dropped(interrupt_state *state)
{
    state->registers.eax = network_manager_get_nic()->interface->frames_dropped;
}
