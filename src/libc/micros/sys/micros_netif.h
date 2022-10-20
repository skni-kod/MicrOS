#ifndef MICROS_SYS_NETIF_H
#define MICROS_SYS_NETIF_H

#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <inet/inet.h>
#include <micros/syscalls.h>
#include "micros_interrupts.h"

uint32_t nic_dropped();

ipv4_addr_t nic_ip();

ipv4_addr_t nic_netmask();

ipv4_addr_t nic_gw();

ipv4_addr_t nic_dns();

#endif