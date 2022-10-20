#include "micros_netif.h"
#include "micros_process.h"

uint32_t nic_dropped()
{
    return micros_interrupt_0a(SYSCALL_NETIF_DROPPED);
}

ipv4_addr_t nic_ip()
{
    ipv4_addr_t addr;
    addr.address = micros_interrupt_0a(SYSCALL_NETIF_GET_IPv4_ADDRESS);
    return addr;
}

ipv4_addr_t nic_netmask()
{
    ipv4_addr_t addr;
    addr.address = micros_interrupt_0a(SYSCALL_NETIF_GET_IPv4_NETMASK);
    return addr;
}

ipv4_addr_t nic_gw()
{
    ipv4_addr_t addr;
    addr.address = micros_interrupt_0a(SYSCALL_NETIF_GET_IPv4_GW);
    return addr;
}

ipv4_addr_t nic_dns()
{
    ipv4_addr_t addr;
    addr.address = micros_interrupt_0a(SYSCALL_NETIF_GET_IPv4_DNS);
    return addr;
}
