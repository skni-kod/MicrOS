/*
    @JakubPrzystasz
    Created: 30.06.2022
    Modified:
*/
#ifndef INET_ICMP_H
#define INET_ICMP_H

#include <stdint.h>

typedef struct icmp_header
{
    uint8_t type;
    uint8_t code;
    uint16_t checksum;
    uint8_t data[];
} __attribute__((packed)) icmp_header_t;

#define ICMP_TYPE_ECHO_REPLY 0             //[RFC792]
#define ICMP_TYPE_DEST_UNREACHABLE 3       //[RFC792]
#define ICMP_TYPE_REDIRECT 5               //[RFC792]
#define ICMP_TYPE_ECHO_REQUEST 8           //[RFC792]
#define ICMP_TYPE_ROUTER_ADVERT 9          //[RFC1256]
#define ICMP_TYPE_ROUTER_SOLICITATION 10   //[RFC1256]
#define ICMP_TYPE_TIME_EXCEEDED 11         //[RFC792]
#define ICMP_TYPE_PARAM_PROBLEM 12         //[RFC792]
#define ICMP_TYPE_TIMESTAMP 13             //[RFC792]
#define ICMP_TYPE_TIMESTAMP_REPLY 14       //[RFC792]
#define ICMP_TYPE_EXTENDED_ECHO_REQUEST 42 //[RFC8335]
#define ICMP_TYPE_EXTENDED_ECHO_REPLY 43   //[RFC8335]

#define ICMP_CODE_NO_CODE 0

#endif