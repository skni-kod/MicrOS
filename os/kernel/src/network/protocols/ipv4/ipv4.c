#include "ipv4.h"

// TODO:
//  add function to track stack, so in ICMP be able to get header size etc
//  add function like ethernet_make_frame, ethernet_send_frame in IPv4,
//  rewrite defintions in protorols
//  eg: IPv4 PROTOCOL_TYPE
//  add ICMP enums

void ipv4_process_packet(nic_data_t *data)
{
    ipv4_packet_t *packet = (ipv4_packet_t *)(data->frame + sizeof(ethernet_frame_t));
    //__ipv4_flip_values(packet);

    // TOS
    // https://datatracker.ietf.org/doc/html/rfc2474
    // https://www.iana.org/assignments/dscp-registry/dscp-registry.xhtml

    switch (packet->protocol)
    {
    case IP_PROTOCOL_ICMP:
    {
        icmp_process_packet(data);
    }
    break;
    case IP_PROTOCOL_UDP:
    {
        udp_process_datagram(data);
    }
    break;
    case IP_PROTOCOL_TCP:
    {
        tcp_process_datagram(data);
    }
    break;
    default:
        break;
    }
}

void __ipv4_flip_values(ipv4_packet_t *packet)
{
    *(packet->version_ihl_ptr) = __uint8_flip(*(packet->version_ihl_ptr));

    packet->length = __uint16_flip(packet->length);
    packet->id = __uint16_flip(packet->id);

    // FLAGS AND FRAGMENT OFFSET
    // uint16_t tmp = (packet->fragment_offset_high << 8);
    // tmp |= packet->fragment_offset_low;
    // tmp |= (packet->flags << 13);
    // memcpy(packet->flags_fragment_ptr, &tmp, sizeof(uint16_t));

    packet->header_checksum = __uint16_flip(packet->header_checksum);
}

void ipv4_checksum(ipv4_packet_t *packet)
{
    packet->header_checksum = 0;
    packet->header_checksum = __ip_wrapsum(__ip_checksum((unsigned char *)packet, sizeof(ipv4_packet_t), 0));
}