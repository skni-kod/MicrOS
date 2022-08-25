#include "udp.h"

void udp_process_datagram(nic_data_t *data)
{
    ipv4_packet_t *packet = (ipv4_packet_t *)(data->frame + sizeof(ethernet_frame_t));
    udp_datagram_t *datagram = (udp_datagram_t *)(data->frame + sizeof(ethernet_frame_t) + sizeof(ipv4_packet_t));

    // echo request
    ethernet_frame_t *frame = ethernet_make_frame(
        data->device->configuration->mac_address,
        ((ethernet_frame_t *)data->frame)->src_mac_addr,
        IPv4_PROTOCOL_TYPE,
        htons(packet->length));
    {
        ipv4_packet_t *reply = (ipv4_packet_t *)frame->data;
        memcpy(reply, packet, sizeof(ipv4_packet_t));
        memcpy(reply->dst_ip, packet->src_ip, IPv4_ADDRESS_LENGTH);
        memcpy(reply->src_ip, packet->dst_ip, IPv4_ADDRESS_LENGTH);
    }

    {
        udp_datagram_t *reply = (udp_datagram_t *)(frame->data + sizeof(ipv4_packet_t));
        reply->dst_port = datagram->src_port;
        reply->src_port = datagram->dst_port;
        reply->length = datagram->length;
        memcpy(reply->data, datagram->data, htons(packet->length) - (sizeof(ipv4_packet_t) + sizeof(udp_datagram_t)));
    }
    nic_data_t tmp;
    tmp.frame = frame;
    ((udp_datagram_t *)(frame->data + sizeof(ipv4_packet_t)))->checksum = udp_checksum(&tmp);
    datagram->checksum = __uint16_flip(datagram->checksum);
    ethernet_send_frame(data->device, htons(packet->length), frame);
}

uint16_t udp_checksum(nic_data_t *data)
{
    ipv4_packet_t *packet = (ipv4_packet_t *)(data->frame + sizeof(ethernet_frame_t));

    /* Check the IP header checksum - it should be zero. */
    if (__ip_wrapsum(__ip_checksum((unsigned char *)packet, sizeof(ipv4_packet_t), 0)) != 0)
        return -1;

    udp_datagram_t *datagram = (udp_datagram_t *)(data->frame + sizeof(ethernet_frame_t) + sizeof(ipv4_packet_t));

    uint32_t len = ntohs(datagram->length) - sizeof(udp_datagram_t);

    uint32_t usum = datagram->checksum;
    datagram->checksum = 0;

    uint32_t sum = __ip_wrapsum(
        __ip_checksum(
            (unsigned char *)datagram, sizeof(udp_datagram_t),
            __ip_checksum(datagram->data, len,
                          __ip_checksum((unsigned char *)&(packet->src_ip), 2 * IPv4_ADDRESS_LENGTH,
                                        IP_PROTOCOL_UDP + ntohs((uint32_t)datagram->length)))));

    if (usum != 0 && usum != sum)
    {
        // 	log_debug("%s: %d bad udp checksums in %d packets",
        // 	    log_procname, udp_packets_bad_checksum,
        // 	    udp_packets_seen);
    }

    return sum;
}

void __udp_flip_values(udp_datagram_t *datagram)
{
    datagram->src_port = __uint16_flip(datagram->src_port);
    datagram->dst_port = __uint16_flip(datagram->dst_port);
    datagram->length = __uint16_flip(datagram->length);
    datagram->checksum = __uint16_flip(datagram->checksum);
}