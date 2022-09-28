#ifndef NETIF_H
#define NETIF_H

#include <cpu/idt/state/interrupt_state.h>
#include <network/network_manager.h>

void syscall_netif_dropped(interrupt_state *state);

#endif