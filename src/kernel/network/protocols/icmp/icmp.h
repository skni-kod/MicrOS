/*
    @JakubPrzystasz
    Created: 30.06.2022
*/
#ifndef icmp_protocol
#define icmp_protocol

#include <inet/ethernet.h>
#include <inet/ipv4.h>
#include <inet/icmp.h>
#include <network/network_utils.h>
#include <network/network_manager.h>

uint32_t icmp_process_datagram(nic_data_t *data);

#endif