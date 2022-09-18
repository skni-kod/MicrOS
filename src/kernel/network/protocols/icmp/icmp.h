/*
    @JakubPrzystasz
    Created: 30.06.2022
*/
#ifndef icmp_protocol
#define icmp_protocol

#include <inet/ethernet.h>
#include <inet/ipv4.h>
#include <inet/icmp.h>

#include "../arp/arp.h"
#include <network/network_utils.h>
#include <network/network_manager.h>

void icmp_process_datagram(nic_data_t *data);

//length - length of IP PACKET
void icmp_checksum(icmp_header_t *datagram, uint32_t packet_length);


#endif