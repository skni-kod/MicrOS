/*
    @JakubPrzystasz
    Created: 17.02.2021
    Modify:
*/
#ifndef ipv4_protocol
#define ipv4_protocol

#include <inet/ethernet.h>
#include <inet/ipv4.h>

#include "../icmp/icmp.h"
#include "../dhcp/dhcp.h"
#include "../udp/udp.h"
#include "../tcp/tcp.h"

#include "../../network_utils.h"
#include "../../network_manager.h"

//! ipv4_process_packet
/*
    Process packet IPv4
*/
void ipv4_process_packet(nic_data_t *data);

//! __ipv4_flip_values
/*
    Flip literal values
*/
void __ipv4_flip_values(ipv4_packet_t *packet);

void ipv4_checksum(ipv4_packet_t *packet);

void ipv4_init();

#endif