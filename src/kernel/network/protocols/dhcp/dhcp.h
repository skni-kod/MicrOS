/*
    @JakubPrzystasz
    Created: 19.10.2022
    Modified: 19.10.2022
*/
#ifndef network_dhcp
#define network_dhcp

#include <process/socket/socket.h>
#include <inet/udp.h>
#include <inet/dhcp.h>
#include <network/network_utils.h>
#include <network/network_manager.h>
#include <debug_helpers/library/kernel_stdio.h>
#include <logger/logger.h>
/**
 * Configures a network interface using DHCP.
 *
 * DHCP operations fall into four phases: server discovery, IP lease offer, IP
 * lease request, and IP lease acknowledgement. This function implements the
 * four phases and configures the given interface once the IP lease
 * acknowledgement is received.
 *
 * @param interface the network interface to use
 * @return zero on success, non zero on fail
 */
uint32_t dhcp_negotiate(net_interface_t *interface);

/**
 * Receives a DHCP packet.
 *
 * @param interface the interface on which the DHCP packet has been received
 * @param packet the DHCP packet
 * @param header the UDP header
 */
void dhcp_receive_packet(net_interface_t *interface,
                         uint8_t *packet,
                         udp_datagram_t *header);

#endif
