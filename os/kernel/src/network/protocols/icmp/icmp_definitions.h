/*
    @JakubPrzystasz
    Created: 30.06.2022
    Modified: 
*/
#ifndef network_icmp_definitions
#define network_icmp_definitions

#include "../../network_definitions.h"
#include <stdint.h>

typedef struct icmp_header
{
    uint8_t type;
    uint8_t code;
    uint16_t checksum;
    uint8_t data[];
} __attribute__((packed)) icmp_header_t;

#endif