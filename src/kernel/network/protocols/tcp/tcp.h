/*
    @JakubPrzystasz
    Created: 17.02.2021
    Modify: 28.09.2022
*/
#ifndef tcp_protocol
#define tcp_protocol

#include <inet/tcp.h>
#include <inet/inet.h>
#include <string.h>
#include <network/network_utils.h>
#include <network/network_manager.h>
#include <process/socket/socket.h>
#include <debug_helpers/library/kernel_stdio.h>

#define SOCKET_BASE_PORT 2022

uint32_t tcp_process_segment(nic_data_t *data);

uint16_t tcp_checksum(ipv4_packet_t *packet);

uint32_t tcp_send_segment(struct socket *socket, uint8_t *data_ptr, uint32_t data_size);

static uint8_t *__tcp_data_ptr(tcp_segment_t *segment);

static uint32_t __tcp_data_size(ipv4_packet_t *packet);

static socket_t *__tcp_get_socket(struct sockaddr_in *addr);

socket_t *tcp_socket_init(socket_t *socket);

int tcp_socket_bind(struct socket *socket, struct sockaddr *addr, int sockaddr_len);

int tcp_socket_listen(struct socket *socket, int backlog);

int tcp_socket_accept(struct socket *socket, struct sockaddr *addr, int sockaddr_len);

int tcp_socket_recv(struct socket *sock, const void *buf, size_t len, int flags);

int tcp_socket_send(struct socket *sock, void *buf, size_t len, int flags);

int tcp_socket_write(struct socket *socket, void *buf, size_t length, struct sockaddr *addr);

int tcp_socket_read(struct socket *socket, void *buf, size_t length, struct sockaddr *addr);

int tcp_socket_connect(struct socket *sock, struct sockaddr *vaddr, int sockaddr_len, int flags);

#endif