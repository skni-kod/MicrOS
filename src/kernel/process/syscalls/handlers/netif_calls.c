#include <process/syscalls/handlers/netif_calls.h>

void syscall_netif_dropped(interrupt_state *state)
{
    state->registers.eax = network_manager_get_nic()->interface->frames_dropped;
}

void syscall_netif_get_ipv4_address(interrupt_state *state)
{
    state->registers.eax = network_manager_get_nic()->interface->ipv4_address.address;
}

void syscall_netif_get_ipv4_netmask(interrupt_state *state)
{
    state->registers.eax = network_manager_get_nic()->interface->ipv4_netmask.address;
}

void syscall_netif_get_ipv4_gw(interrupt_state *state)
{
    state->registers.eax = network_manager_get_nic()->interface->ipv4_gateway.address;
}


void syscall_netif_get_ipv4_dns(interrupt_state *state)
{
    state->registers.eax = network_manager_get_nic()->interface->ipv4_dns.address;
}

