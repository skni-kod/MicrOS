#ifndef MICROS_SOCKET_H
#define MICROS_SOCKET_H

// Domains
#define AF_INET 2

// Types
#define SOCK_DGRAM 2
#define SOCK_RAW 3

// Protocols
#define IPPROTO_IP 0
#define IPPROTO_ICMP 1
#define IPPROTO_TCP 6
#define IPPROTO_UDP 17
#define IPPROTO_RAW 255

#define INADDR_LOOPBACK 0xfe000001
#define INADDR_ANY 0x0
#define INADDR_BROADCAST 0xffffffff

typedef uint16_t sa_family_t;

typedef uint32_t socklen_t;

struct sockaddr
{
  sa_family_t sa_family;
  int8_t sa_data[14];
};

#endif