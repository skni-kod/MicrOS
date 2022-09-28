/*
	@JakubPrzystasz
*/
#ifndef SOCKET_H
#define SOCKET_H

#define SOCKET_BUFFER_ENTRY_SIZE 1500
#define SOCKET_BUFFER_ENTRY_COUNT 1024
#define SOCKET_DESCRIPTORS_COUNT 64

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <memory/heap/heap.h>
#include <micros/socket.h>
#include <micros/sys/micros_socket.h>
#include <network/network_device.h>

struct socket;

typedef struct socket_buffer
{
	// circular buffer
	uint16_t length;
	size_t entry_size;
	uint16_t read;
	uint16_t write;
	uint8_t entries[];
} socket_buffer_t;

typedef struct socket_entry
{
	size_t size;
	struct sockaddr addr;
	uint8_t data[];
} __attribute__((packed)) socket_entry_t;

struct proto_ops
{
	int (*release)(struct socket *sock);
	int (*bind)(struct socket *sock,
				struct sockaddr *myaddr,
				int sockaddr_len);
	int (*connect)(struct socket *sock,
				   struct sockaddr *vaddr,
				   int sockaddr_len, int flags);
	int (*socketpair)(struct socket *sock1,
					  struct socket *sock2);
	int (*accept)(struct socket *sock,
				  struct socket *newsock, int flags);
	int (*getname)(struct socket *sock,
				   struct sockaddr *addr,
				   int *sockaddr_len, int peer);
	unsigned int (*poll)(struct file *file, struct socket *sock,
						 struct poll_table_struct *wait);
	int (*ioctl)(struct socket *sock, unsigned int cmd,
				 unsigned long arg);
	int (*listen)(struct socket *sock, int len);
	int (*shutdown)(struct socket *sock, int flags);

	int (*sendmsg)(struct socket *sock, struct msghdr *m,
				   size_t total_len);

	int (*recvmsg)(struct socket *sock, struct msghdr *m,
				   size_t total_len, int flags);

	int (*send)(struct socket *sock, const void *buf, size_t len,
				  int flags);

	int (*sendto)(struct socket *sock, const void *buf, size_t len,
				  int flags, const struct sockaddr *to,
				  socklen_t tolen);

	int (*recv)(struct socket *sock, void *buf, size_t len, int flags);

	int (*recvfrom)(struct socket *sock, void *buf, size_t len, int flags,
					struct sockaddr *from, socklen_t *fromlen);

	int (*write)(struct socket *sock, void *buf, size_t len, struct sockaddr *addr);

	int (*read)(struct socket *sock, void *buf, size_t len, struct sockaddr *addr);
};

typedef struct socket
{
	socket_state_t state;
	socket_type_t type;
	socket_domain_t domain;
	uint32_t protocol;
	uint32_t flags;
	struct proto_ops *ops;
	void *sk;
} socket_t;

typedef struct udp_socket
{
	struct sockaddr_in local;
	struct sockaddr_in remote;
	socket_buffer_t *buffer;
	net_device_t *device;
} udp_socket_t;

typedef struct tcp_socket
{
	struct sockaddr_in local;
	struct sockaddr_in remote;
	socket_buffer_t *tx;
	socket_buffer_t *rx;
	net_device_t *device;
} tcp_socket_t;

int k_socket(int domain, int type, int protocol);

int k_bind(int s, struct sockaddr *addr, int addrlen);

int k_recv(int s, void *buf, size_t len, int flags);

int k_recvfrom(int s, void *buf, size_t len, int flags, struct sockaddr *from, int *fromlen);

int k_sendto(int s, const void *buf, size_t len, int flags, const struct sockaddr *to, int tolen);

int k_send(int s, const void *buf, size_t len, int flags);

int k_listen(int s, int backlog);

int k_accept(int s, struct sockaddr *addr, int *addrlen);

// kernel specific:

int socket_create_descriptor(int domain, int type, int protocol);

socket_buffer_t *socket_init_buffer(socket_buffer_t *buffer, size_t entry_size, uint32_t entry_count);

socket_t *socket_get_descriptor(int socket);

int socket_read(socket_t *socket, void *data, size_t length, struct sockaddr *to);

int socket_write(socket_t *socket, void *data, size_t length, struct sockaddr *from);

socket_t *socket_descriptor_lookup(int domain, int type, int protocol, struct sockaddr *addr);

#endif
