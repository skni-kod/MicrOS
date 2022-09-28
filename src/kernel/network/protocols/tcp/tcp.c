#include "tcp.h"

#include <debug_helpers/library/kernel_stdio.h>

void tcp_process_segment(nic_data_t *data)
{
    ipv4_packet_t *packet = (ipv4_packet_t *)(data->frame + sizeof(ethernet_frame_t));
    tcp_segment_t *segment = packet->data;

#ifndef TRUST_ME_BRO
    // verify checksum
    uint16_t checksum = segment->checksum;
    tcp_checksum(packet);
    if(checksum != tcp_checksum(packet))
        return;
#endif


    struct sockaddr_in addr = {
        .sin_addr.s_addr = (in_addr_t)packet->dst.address,
        .sin_port = segment->dst_port};

    // first look for open socket, so forward there incoming data:
    socket_descriptor_t *socket = socket_descriptor_lookup(AF_INET, SOCK_STREAM, IPPROTO_TCP, &addr);

    if (socket)
    {
        addr.sin_addr.s_addr = (in_addr_t)packet->src.address;
        addr.sin_port = segment->src_port;
    }
    else
    {
    }
}

uint16_t tcp_checksum(ipv4_packet_t *packet)
{
    /* Check the IP header checksum - it should be zero. */
    if (__ip_wrapsum(__ip_checksum((unsigned char *)packet, sizeof(ipv4_packet_t), 0)) != 0)
        return -1;

    tcp_segment_t *segment = (tcp_segment_t *)packet->data;

    uint32_t len = ntohs(packet->length) - (packet->ihl << 2);

    segment->checksum = 0;

    uint32_t sum = __ip_wrapsum(
        __ip_checksum(
            (unsigned char *)segment, sizeof(tcp_segment_t),
            __ip_checksum(segment->options_data, len - sizeof(tcp_segment_t),
                          __ip_checksum((unsigned char *)&(packet->src), 2 * sizeof(ipv4_addr_t),
                                        IP_PROTOCOL_TCP + len))));
    return segment->checksum = sum;
}

nic_data_t *tcp_create_segment(net_device_t *device, ipv4_addr_t dst_addr, uint16_t dst_port, uint16_t src_port, uint32_t data_size)
{
    // nic_data_t *data = ipv4_create_packet(device, IP_PROTOCOL_UDP, dst_addr, data_size + sizeof(tcp_segment_t));
    // ipv4_packet_t *packet = data->frame + sizeof(ethernet_frame_t);
    // udp_datagram_t *datagram = &packet->data;
    // datagram->dst_port = dst_port;
    // datagram->src_port = src_port;
    // datagram->length = htons((data_size + sizeof(udp_datagram_t)));
    // return data;
}

uint32_t tcp_send_segment(nic_data_t *data)
{
    ipv4_packet_t *packet = data->frame + sizeof(ethernet_frame_t);
    udp_checksum(packet);

    return ipv4_send_packet(data);
}

static uint8_t *__tcp_data_ptr(tcp_segment_t *segment)
{
    return (uint8_t *)segment + (segment->offset << 2);
}

static uint32_t __tcp_data_size(ipv4_packet_t *packet)
{
    tcp_segment_t *segment = (tcp_segment_t *)(packet->data);
    return ntohs(packet->length) - (segment->offset << 2);
}
