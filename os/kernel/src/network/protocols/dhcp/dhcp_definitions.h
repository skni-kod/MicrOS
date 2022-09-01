/*
    @JakubPrzystasz
    Created: 01.09.2022
    Modified: 
*/
#ifndef network_dhcp_definitions
#define network_dhcp_definitions

#include "../../network_definitions.h"
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