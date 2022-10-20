#ifndef NETIF_H
#define NETIF_H

#include <cpu/idt/state/interrupt_state.h>
#include <network/network_manager.h>

void syscall_netif_dropped(interrupt_state *state);

void syscall_netif_get_ipv4_address(interrupt_state *state);

void syscall_netif_get_ipv4_netmask(interrupt_state *state);

void syscall_netif_get_ipv4_gw(interrupt_state *state);

void syscall_netif_get_ipv4_dns(interrupt_state *state);

#endif