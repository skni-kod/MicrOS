/*
    @JakubPrzystasz
    Created: 19.10.2022
    Modified: 19.10.2022
*/
#ifndef network_dns
#define network_dns

#include <process/socket/socket.h>
#include <inet/udp.h>
#include <inet/dns.h>
#include <network/network_utils.h>
#include <network/network_manager.h>
#include <debug_helpers/library/kernel_stdio.h>
#include <logger/logger.h>
#include <string.h>
/**
 * Performs a DNS lookup.
 *
 * @param domain the domain name that is to be looked up
 * @param ip the receiving array for the IPv4 address
 * @return `0` on success, something else on error
 */
ipv4_addr_t dns_lookup(const char* domain);

#endif
