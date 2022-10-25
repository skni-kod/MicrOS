/*
    @JakubPrzystasz
    Created: 25.10.2022
    Modified:
*/
#ifndef INET_DNS_H
#define INET_DNS_H

#include <stdint.h>
#include "ipv4.h"


#define DNS_TYPE_A   0x0001
#define DNS_CLASS_IN 0x0001

#define DNS_ERR_SOCK      1
#define DNS_ERR_SEND      2
#define DNS_ERR_RECV      3
#define DNS_ERR_CLASS     4
#define DNS_ERR_NO_ANSWER 5

typedef struct dns_header
{
  uint16_t id;
  uint16_t flags;
  uint16_t qdcount;
  uint16_t ancount;
  uint16_t nscount;
  uint16_t arcount;
} __attribute__((packed)) dns_header_t;

typedef struct dns_answer_header
{
  uint16_t name;
  uint16_t type;
  uint16_t class;
  uint32_t ttl;
  uint16_t data_len;
} __attribute__((packed)) dns_answer_header_t;


#endif