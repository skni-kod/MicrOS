/*
    @JakubPrzystasz
*/
#ifndef SOCKET_H
#define SOCKET_H

#define SOCKET_BUFFER_SIZE 1024 * 1024
#define SOCKET_DESCRIPTORS 64
#define SOCKET_BASE_PORT 2022

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <micros/sys/micros_socket.h>
#include <micros/socket.h>
#include <memory/heap/heap.h>
#include <network/network_manager.h>

typedef struct socket_entry
{
  // circular buffer
  uint16_t length;
  uint16_t read;
  uint16_t write;
  uint8_t buffer[];
} socket_entry_t;

typedef struct socket_descriptor
{
  bool used;
  uint32_t domain;
  uint32_t type;
  uint32_t protocol;
  uint16_t port;
  socket_entry_t *entry;
  struct sockaddr_in addr;
  socklen_t addr_len;
} socket_descriptor_t;

int socket(int domain, int type, int protocol);

ssize_t recv(int s, void *buf, size_t len, int flags);

ssize_t recvfrom(int s, void *buf, size_t len, int flags, struct sockaddr *from, socklen_t *fromlen);

int bind(int s, struct sockaddr *my_addr, socklen_t addrlen);

ssize_t sendto(int s, const void *buf, size_t len, int flags, const struct sockaddr *to, socklen_t tolen);

int socket_create_descriptor(int domain, int type, int protocol);

socket_descriptor_t *socket_get_descriptor(uint32_t id);

size_t socket_entry_read(socket_entry_t *entry, void *buffer, size_t length);

size_t socket_entry_write(socket_entry_t *entry, void *buffer, size_t length);

socket_descriptor_t *socket_descriptor_lookup(int domain, int type, int protocol, uint16_t port);

#endif
