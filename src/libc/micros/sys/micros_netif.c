#include "micros_netif.h"
#include "micros_process.h"

uint32_t micros_netif_get(uint32_t id, net_interface_t *interface)
{
    return micros_interrupt_2a(SYSCALL_NETIF_GET, id, interface);
}

uint32_t micros_netif_set(uint32_t id, net_interface_t *interface)
{
    return micros_interrupt_2a(SYSCALL_NETIF_SET, id, interface);
}

uint32_t micros_netif_count()
{
    return micros_interrupt_0a(SYSCALL_NETIF_COUNT);
}

ipv4_addr_t dns_lookup(const char *hostname)
{
    return (ipv4_addr_t){
        .address = micros_interrupt_1a(SYSCALL_DNS_LOOKUP, hostname),
    };
}