#include "micros_netif.h"
#include "micros_process.h"

uint32_t nic_dropped()
{
    return micros_interrupt_0a(SYSCALL_NETIF_DROPPED);
}

ipv4_addr_t nic_ip()
{
    return (ipv4_addr_t){
        .address = micros_interrupt_0a(SYSCALL_NETIF_GET_IPv4_ADDRESS),
    };
}

ipv4_addr_t nic_netmask()
{
    return (ipv4_addr_t){
        .address = micros_interrupt_0a(SYSCALL_NETIF_GET_IPv4_NETMASK),
    };
}

ipv4_addr_t nic_gw()
{
    return (ipv4_addr_t){
        .address = micros_interrupt_0a(SYSCALL_NETIF_GET_IPv4_GW),
    };
}

ipv4_addr_t nic_dns()
{
    return (ipv4_addr_t){
        .address = micros_interrupt_0a(SYSCALL_NETIF_GET_IPv4_DNS),
    };
}

ipv4_addr_t dns_lookup(const char *hostname)
{
    return (ipv4_addr_t){
        .address = micros_interrupt_1a(SYSCALL_DNS_LOOKUP, hostname),
    };
}