/*
	@JakubPrzystasz
*/
#ifndef SOCKET_H
#define SOCKET_H

#define SOCKET_BUFFER_ENTRY_SIZE 1500
#define SOCKET_BUFFER_ENTRY_COUNT 1024
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
	size_t size;
	struct sockaddr_in addr;
	uint8_t data[];
} __attribute__((packed)) socket_entry_t;

typedef struct socket_buffer
{
	// circular buffer
	uint16_t length;
	size_t entry_size;
	uint16_t read;
	uint16_t write;
	uint8_t entries[];
} socket_buffer_t;

typedef struct socket_descriptor
{
	bool used;
	uint32_t domain;
	uint32_t type;
	uint32_t protocol;
	uint16_t port;
	struct sockaddr_in addr;
	socklen_t addr_len;
	socket_buffer_t *buffer;
} socket_descriptor_t;

// unix

int socket(int domain, int type, int protocol);

uint32_t recv(int s, void *buf, size_t len, int flags);

uint32_t recvfrom(int s, void *buf, size_t len, int flags, struct sockaddr *from, socklen_t *fromlen);

int bind(int s, struct sockaddr *my_addr, socklen_t addrlen);

uint32_t sendto(int s, const void *buf, size_t len, int flags, const struct sockaddr *to, socklen_t tolen);

uint32_t send(int s, const void *buf, size_t len, int flags);

int listen(int s, int backlog);

// kernel specific:

int socket_create_descriptor(int domain, int type, int protocol);

socket_buffer_t *socket_init_buffer(socket_buffer_t *buffer, size_t entry_size, uint32_t entry_count);

socket_descriptor_t *socket_get_descriptor(uint32_t id);

uint32_t socket_read(socket_descriptor_t *socket, struct sockaddr_in *addr, void *data, size_t length);

uint32_t socket_write(socket_descriptor_t *socket, struct sockaddr_in *addr, void *data, size_t length);

socket_descriptor_t *socket_descriptor_lookup(int domain, int type, int protocol, struct sockaddr_in *addr);

#endif
