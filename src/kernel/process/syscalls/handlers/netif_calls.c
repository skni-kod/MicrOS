#include <process/syscalls/handlers/netif_calls.h>

void syscall_netif_get(interrupt_state *state)
{
    kvector *devices = network_manager_get_devices(devices);
    if (state->registers.ebx >= devices->count)
    {
        state->registers.eax = 0;
        return;
    }
    if (state->registers.ecx)
        memcpy((void *)state->registers.ecx, ((net_device_t *)devices->data[state->registers.ebx])->interface, sizeof(net_interface_t));
}

void syscall_netif_set(interrupt_state *state)
{
    kvector *devices = network_manager_get_devices(devices);
    if (state->registers.ebx >= devices->count)
    {
        state->registers.eax = 0;
        return;
    }
    if (state->registers.ecx)
        memcpy(((net_device_t *)devices->data[state->registers.ebx])->interface, (void *)state->registers.ecx, sizeof(net_interface_t) - (sizeof(uint32_t) * 5));
}

void syscall_netif_count(interrupt_state *state)
{
    kvector *devices;
    state->registers.eax = network_manager_get_devices(devices)->count;
}

void syscall_dns_lookup(interrupt_state *state)
{
    state->registers.eax = dns_lookup(state->registers.ebx).value;
}
