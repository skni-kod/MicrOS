/*
    @JakubPrzystasz
    Created: 01.09.2022
    Modified: 
*/
#ifndef INET_DHCP_H
#define INET_DHCP_H

#include <stdint.h>

typedef struct dhcp_message
{
    uint8_t htype;
    uint8_t op;
    uint8_t hops;
    uint8_t hlen;
    uint32_t xid;
    uint16_t secs;
    uint16_t flags;
    uint32_t ciaddr;
    uint32_t yiaddr;
    uint32_t siaddr;
    uint32_t giaddr;
    uint32_t chaddr[4];
    uint32_t sname[16];
    uint32_t file[32];
    uint8_t options[];
} __attribute__((packed)) dhcp_message_t;

#endif