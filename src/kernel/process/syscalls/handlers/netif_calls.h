#ifndef NETIF_H
#define NETIF_H

#include <cpu/idt/state/interrupt_state.h>
#include <network/network_manager.h>
#include <network/protocols/dns/dns.h>
#include <string.h>

void syscall_netif_get(interrupt_state *state);

void syscall_netif_set(interrupt_state *state);

void syscall_netif_count(interrupt_state *state);

void syscall_dns_lookup(interrupt_state *state);

#endif