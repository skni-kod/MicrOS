#ifndef MICROS_SOCKET_H
#define MICROS_SOCKET_H

typedef uint32_t socklen_t;

typedef enum socket_domain
{
  AF_INET = 2,
} socket_domain_t;

typedef enum socket_state
{
  SS_FREE = 0,
  SS_UNCONNECTED = 1,
  SS_CONNECTING = 2,
  SS_CONNECTED = 3,
  SS_DISCONNECTING = 4,
  SS_UNDEFINED = 0xFFFF,
} socket_state_t;

typedef enum socket_type
{
  SOCK_DGRAM = 1,
  SOCK_STREAM = 2,
  SOCK_RAW = 3,
  SOCK_RDM = 4,
  SOCK_SEQPACKET = 5,
  SOCK_DCCP = 6,
  SOCK_PACKET = 10,
  SOCK_UNDEFINED = 0xFFFF,
} socket_type_t;

struct sockaddr
{
  uint16_t sa_family;
  uint8_t sa_data[14];
};

#endif