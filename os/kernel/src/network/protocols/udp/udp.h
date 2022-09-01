/*
    @JakubPrzystasz
    Created: 17.02.2021
    Modify:
*/
#ifndef udp_protocol
#define udp_protocol

#include "udp_definitions.h"
#include "../../network_utils.h"
#include "../../network_manager.h"

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

#endif