/*
    @JakubPrzystasz
    Created: 17.02.2021
    Modify: 28.09.2022
*/
#ifndef tcp_protocol
#define tcp_protocol

#include <inet/tcp.h>
#include <inet/inet.h>
#include <network/network_utils.h>
#include <network/network_manager.h>
#include <process/socket/socket.h>

#define SOCKET_BASE_PORT 2022

uint32_t tcp_process_segment(nic_data_t *data);

uint16_t tcp_checksum(ipv4_packet_t *packet);

nic_data_t *tcp_create_segment(net_device_t *device, ipv4_addr_t dst_addr, uint16_t dst_port, uint16_t src_port, uint32_t data_size);

uint32_t tcp_send_segment(nic_data_t *data);

static uint8_t *__tcp_data_ptr(tcp_segment_t *segment);

static uint32_t __tcp_data_size(ipv4_packet_t *packet);

static uint32_t __tcp_options_size(ipv4_packet_t *packet);

socket_t *tcp_socket_init(socket_t *socket);

int tcp_socket_bind(struct socket *sock, struct sockaddr *addr, int sockaddr_len);

int tcp_socket_listen(struct socket *sock, int len);


#endif