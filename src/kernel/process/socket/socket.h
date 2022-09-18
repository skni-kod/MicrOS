/*
    @JakubPrzystasz
*/
#ifndef SOCKET_H
#define SOKCET_H

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <micros/micros_socket.h>
#include <memory/heap/heap.h>

typedef struct sock_entry
{
  // circular buffer
  uint16_t len;
  uint16_t read;
  uint16_t write;
  uint8_t buffer[];
} sock_entry_t;

typedef struct descriptor
{
  bool used;
  uint32_t domain;
  uint32_t type;
  uint32_t protocol;
  uint16_t port;
  sock_entry_t *entry;
} descriptor_t;

uint32_t create_socket_descriptor(uint32_t domain,
                                  uint32_t type,
                                  uint32_t protocol,
                                  uint32_t port);

uint32_t descriptor_udp_lookup(uint16_t port);

descriptor_t *get_descriptor(uint32_t id);

uint32_t k_socket(uint32_t domain, uint32_t type, uint32_t protocol, uint32_t port);

uint32_t k_recvfrom(uint32_t sock, void *buffer, size_t len);

uint32_t entry_read(sock_entry_t *entry, uint8_t *buffer, size_t len);

uint32_t entry_write(sock_entry_t *entry, uint8_t *buffer, size_t len);

#endif
