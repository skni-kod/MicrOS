#ifndef MICROS_SOCKET_H
#define MICROS_SOCKET_H

#include <stdint.h>
#include <stddef.h>
#include "micros_interrupts.h"

// Domains
#define AF_INET 2
// Types
#define SOCK_DGRAM 2
#define SOCK_RAW 3
// Protocols
#define IPPROTO_IP 0
#define IPPROTO_ICMP 1
#define IPPROTO_UDP 17

uint32_t micros_socket(uint32_t domain, uint32_t type, uint32_t protocol, uint32_t port);

uint32_t micros_recv(uint32_t sock, void* buffer, size_t len);
#endif