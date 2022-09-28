#include "udp.h"

void udp_process_datagram(nic_data_t *data)
{
    ipv4_packet_t *packet = (ipv4_packet_t *)(data->frame + sizeof(ethernet_frame_t));
    udp_datagram_t *datagram = packet->data;

#ifndef TRUST_ME_BRO
    // verify checksum
    uint16_t checksum = datagram->checksum;
    if (checksum != udp_checksum(packet))
        return;
#endif

    struct sockaddr_in addr = {
        .sin_addr.s_addr = (in_addr_t)packet->dst.address,
        .sin_port = datagram->dst_port};

    // first look for open socket, so forward there incoming data:
    socket_descriptor_t *socket = socket_descriptor_lookup(AF_INET, SOCK_DGRAM, IPPROTO_UDP, &addr);

    if (socket)
    {
        addr.sin_addr.s_addr = (in_addr_t)packet->src.address;
        addr.sin_port = datagram->src_port;
        socket_write(socket, &addr, datagram->data, ntohs(datagram->length) - sizeof(udp_datagram_t));
    }
    else
    {
    }
    // otherwise drop it
}

uint16_t udp_checksum(ipv4_packet_t *packet)
{
    /* Check the IP header checksum - it should be zero. */
    if (__ip_wrapsum(__ip_checksum((unsigned char *)packet, sizeof(ipv4_packet_t), 0)) != 0)
        return -1;

    udp_datagram_t *datagram = (udp_datagram_t *)packet->data;

    uint32_t len = ntohs(datagram->length) - sizeof(udp_datagram_t);

    datagram->checksum = 0;

    uint32_t sum = __ip_wrapsum(
        __ip_checksum(
            (unsigned char *)datagram, sizeof(udp_datagram_t),
            __ip_checksum(datagram->data, len,
                          __ip_checksum((unsigned char *)&(packet->src), 2 * sizeof(ipv4_addr_t),
                                        IP_PROTOCOL_UDP + ntohs((uint32_t)datagram->length)))));
    return datagram->checksum = sum;
}

nic_data_t *udp_create_datagram(net_device_t *device, ipv4_addr_t dst_addr, uint16_t dst_port, uint16_t src_port, uint32_t data_size)
{
    nic_data_t *data = ipv4_create_packet(device, IP_PROTOCOL_UDP, dst_addr, data_size + sizeof(udp_datagram_t));
    ipv4_packet_t *packet = data->frame + sizeof(ethernet_frame_t);
    udp_datagram_t *datagram = &packet->data;
    datagram->dst_port = dst_port;
    datagram->src_port = src_port;
    datagram->length = htons((data_size + sizeof(udp_datagram_t)));
    return data;
}

uint32_t udp_send_datagram(nic_data_t *data)
{
    ipv4_packet_t *packet = data->frame + sizeof(ethernet_frame_t);
    udp_checksum(packet);

    return ipv4_send_packet(data);
}
