#include "udp.h"

uint16_t compute_udp_checksum(ipv4_packet_t *pIph, unsigned short *ipPayload);

void udp_process_datagram(nic_data_t *data)
{
    ipv4_packet_t *packet = (ipv4_packet_t *)(data->frame + sizeof(ethernet_frame_t));
    udp_datagram_t *datagram = (udp_datagram_t *)(data->frame + sizeof(ethernet_frame_t) + sizeof(ipv4_packet_t));
    char str[120];
    // kernel_sprintf(str, "%d.%d.%d.%d:%d",
    //                packet->dst_ip[0],
    //                packet->dst_ip[1],
    //                packet->dst_ip[2],
    //                packet->dst_ip[3],
    //                datagram->dst_port);
    // logger_log_info(str);
    kernel_sprintf(str, "%x - %x", __uint16_flip(datagram->checksum), compute_udp_checksum(packet, datagram));
    logger_log_info(str);

    datagram->src_port = __uint16_flip(datagram->src_port);
    datagram->dst_port = __uint16_flip(datagram->dst_port);
    datagram->length = __uint16_flip(datagram->length);
}

/* set tcp checksum: given IP header and UDP datagram */
uint16_t compute_udp_checksum(ipv4_packet_t *pIph, unsigned short *ipPayload)
{

    register unsigned long sum = 0;
    udp_datagram_t *udphdrp = (udp_datagram_t *)(ipPayload);
    unsigned short udpLen = __uint16_flip(udphdrp->length);
    // the source ip
    sum += ((*(uint32_t*)pIph->src_ip) >> 16) & 0xFFFF;
    sum += ((uint32_t)pIph->src_ip) & 0xFFFF;
    // the dest ip
    sum += ((*(uint32_t*)pIph->dst_ip) >> 16) & 0xFFFF;
    sum += ((uint32_t)pIph->dst_ip) & 0xFFFF;
    // protocol and reserved: 17
    sum += __uint16_flip((uint16_t)IP_PROTOCOL_UDP);
    // the length
    sum += udphdrp->length;

    uint16_t checksum = udphdrp->checksum;
    udphdrp->checksum = 0;

    while (udpLen > 1)
    {
        sum += *ipPayload++;
        udpLen -= 2;
    }

    if (udpLen > 0)
        sum += ((*ipPayload) & __uint16_flip(0xFF00));

    while (sum >> 16)
        sum = (sum & 0xffff) + (sum >> 16);

    sum = ~sum;

    udphdrp->checksum = checksum;
    return ((unsigned short)sum == 0x0000) ? 0xFFFF : (unsigned short)sum;
}