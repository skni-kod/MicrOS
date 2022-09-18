#include "udp.h"

void udp_process_datagram(nic_data_t *data)
{
    ipv4_packet_t *packet = (ipv4_packet_t *)(data->frame + sizeof(ethernet_frame_t));
    udp_datagram_t *datagram = (udp_datagram_t *)(data->frame + sizeof(ethernet_frame_t) + sizeof(ipv4_packet_t));

    uint32_t sock = descriptor_udp_lookup(5555);
    if(sock)
        k_sendto(sock,datagram->data,ntohs(datagram->length) - sizeof(udp_datagram_t));  

    // ethernet_frame_t *frame = ethernet_create_frame(
    //     &data->device->interface->mac,
    //     &((ethernet_frame_t *)data->frame)->src,
    //     IPv4_PROTOCOL_TYPE,
    //     htons(packet->length));
    
    // ipv4_packet_t *ip_reply = (ipv4_packet_t *)frame->data;
    // {
    //     memcpy(ip_reply, packet, sizeof(ipv4_packet_t));
    //     packet->flags_mf = IPv4_FLAG_DONT_FRAGMENT;
    //     memcpy(&ip_reply->dst, &packet->src, IPv4_ADDRESS_LENGTH);
    //     memcpy(&ip_reply->src, &packet->dst, IPv4_ADDRESS_LENGTH);
    // }

    // {
    //     udp_datagram_t *reply = (udp_datagram_t *)(frame->data + sizeof(ipv4_packet_t));
    //     reply->dst_port = datagram->src_port;
    //     reply->src_port = datagram->dst_port;
    //     reply->length = datagram->length;
    //     memcpy(reply->data, datagram->data, htons(packet->length) - (sizeof(ipv4_packet_t) + sizeof(udp_datagram_t)));
    // }

    // udp_checksum(ip_reply);
    // ethernet_send_frame(data->device, htons(packet->length), frame);
}

uint16_t udp_checksum(ipv4_packet_t *packet)
{
    /* Check the IP header checksum - it should be zero. */
    if (__ip_wrapsum(__ip_checksum((unsigned char *)packet, sizeof(ipv4_packet_t), 0)) != 0)
        return -1;

    udp_datagram_t *datagram = (udp_datagram_t *)packet->data;

    uint32_t len = ntohs(datagram->length) - sizeof(udp_datagram_t);

    uint32_t usum = datagram->checksum;
    datagram->checksum = 0;

    uint32_t sum = __ip_wrapsum(
        __ip_checksum(
            (unsigned char *)datagram, sizeof(udp_datagram_t),
            __ip_checksum(datagram->data, len,
                          __ip_checksum((unsigned char *)&(packet->src), 2 * IPv4_ADDRESS_LENGTH,
                                        IP_PROTOCOL_UDP + ntohs((uint32_t)datagram->length)))));
    datagram->checksum = sum;
    return sum;
}
