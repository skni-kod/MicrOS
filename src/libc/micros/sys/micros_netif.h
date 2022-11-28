#ifndef MICROS_SYS_NETIF_H
#define MICROS_SYS_NETIF_H

#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <micros/network_device.h>
#include <micros/syscalls.h>
#include "micros_interrupts.h"

uint32_t micros_nic_get(uint32_t id, net_interface_t *interface);
uint32_t micros_nic_set(uint32_t id, net_interface_t *interface);
uint32_t micros_nic_count();

ipv4_addr_t dns_lookup(const char *hostname);

#endif