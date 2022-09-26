/*
    @JakubPrzystasz
    Created: 17.02.2021
    Modify:
*/
#ifndef udp_protocol
#define udp_protocol

#include <inet/udp.h>
#include <inet/inet.h>
#include <network/network_utils.h>
#include <network/network_manager.h>
#include <process/socket/socket.h>

//! udp_process_datagram
/*
    Process UDP datagram
*/
void udp_process_datagram(nic_data_t *data);

//! udp_checksum
/*
    Returns datagram checksum
*/
uint16_t udp_checksum(ipv4_packet_t *packet);

nic_data_t *udp_create_datagram(net_device_t *device, ipv4_addr_t dst_addr, uint16_t dst_port, uint16_t src_port, uint32_t data_size);

uint32_t udp_send_datagram(nic_data_t *data);

#endif