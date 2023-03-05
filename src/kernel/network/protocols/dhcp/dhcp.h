/*
    @JakubPrzystasz
    Created: 19.10.2022
    Modified: 19.10.2022
*/
#ifndef network_dhcp
#define network_dhcp

#include <micros/socket.h>
#include <micros/sys/micros_process.h>
#include <micros/sys/micros_netif.h>
#include <micros/network_device.h>
#include <inet/udp.h>
#include <inet/dhcp.h>
#include <stdio.h>

#define dhcp_add_option(msg, option, value, length) _Generic((value), uint8_t                         \
                                                             : __dhcp_add_option(msg, option, value), \
                                                               default                                \
                                                             : __dhcp_add_option_ptr(msg, option, value, length))

static void __dhcp_add_option(dhcp_message_t *msg, uint8_t option, uint8_t value);
static void __dhcp_add_option_ptr(dhcp_message_t *msg, uint8_t option, uint8_t value[], uint8_t value_length);

uint32_t dhcp_negotiate(net_interface_t *interface);
static void dhcp_discover(net_interface_t *interface);
static void dhcp_read_option(dhcp_message_t *msg, uint8_t code, uint8_t *buf, uint16_t len);
static int dhcp_process_offer(net_interface_t *interface, dhcp_message_t *dhcp_offer);

#endif
