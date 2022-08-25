#include "tcp.h"

void tcp_process_datagram(nic_data_t *data)
{
}

uint16_t tcp_checksum(nic_data_t *data)
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
                                        IP_PROTOCOL_TCP + ntohs((uint32_t)datagram->length)))));

    if (usum != 0 && usum != sum)
    {
        // 	log_debug("%s: %d bad udp checksums in %d packets",
        // 	    log_procname, udp_packets_bad_checksum,
        // 	    udp_packets_seen);
    }

    return sum;
}