#include "udp.h"

uint32_t
checksum(unsigned char *buf, uint32_t nbytes, uint32_t sum)
{
	unsigned int	 i;

	/* Checksum all the pairs of bytes first. */
	for (i = 0; i < (nbytes & ~1U); i += 2) {
		sum += (uint16_t)ntohs(*((uint16_t *)(buf + i)));
		if (sum > 0xFFFF)
			sum -= 0xFFFF;
	}

	/*
	 * If there's a single byte left over, checksum it, too.
	 * Network byte order is big-endian, so the remaining byte is
	 * the high byte.
	 */
	if (i < nbytes) {
		sum += buf[i] << 8;
		if (sum > 0xFFFF)
			sum -= 0xFFFF;
	}

	return sum;
}

uint32_t
wrapsum(uint32_t sum)
{
	sum = ~sum & 0xFFFF;
	return htons((uint16_t)sum);
}

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
    uint16_t tmp1 = (datagram->checksum);
    uint16_t tmp2 = compute_udp_checksum(data);
    kernel_sprintf(str, "ori: %x calcu: %x", htons(tmp1), htons(tmp2));
    logger_log_info(str);

    // echo request
    ethernet_frame_t *frame = ethernet_make_frame(
        data->device->configuration->mac_address,
        ((ethernet_frame_t *)data->frame)->src_mac_addr,
        IPv4_PROTOCOL_TYPE,
        htons(packet->length));
    {
        ipv4_packet_t *reply = (ipv4_packet_t *)frame->data;
        memcpy(reply, packet, sizeof(ipv4_packet_t));
        //__ipv4_flip_values(reply);
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
    //__udp_flip_values((udp_datagram_t *)(frame->data + sizeof(ipv4_packet_t)));
    nic_data_t tmp;
    tmp.frame = frame;
    ((udp_datagram_t *)(frame->data + sizeof(ipv4_packet_t)))->checksum = compute_udp_checksum(&tmp);
    datagram->checksum = __uint16_flip(datagram->checksum);
    ethernet_send_frame(data->device, htons(packet->length), frame);
}

/* set tcp checksum: given IP header and UDP datagram */
uint16_t compute_udp_checksum(nic_data_t *data)
{
    ipv4_packet_t *packet = (ipv4_packet_t *)(data->frame + sizeof(ethernet_frame_t));

    /* Check the IP header checksum - it should be zero. */
	if (wrapsum(checksum((unsigned char *)packet, sizeof(ipv4_packet_t), 0)) != 0) {
		//bad checksum
		return -1;
	}

	// if (ntohs(packet->length) != buflen)
	// 	log_debug("%s: ip length %hu disagrees with bytes received %d",
	// 	    log_procname, ntohs(ip->ip_len), buflen);

	/* Assure that the entire IP packet is within the buffer. */
	// if (ntohs(ip->ip_len) > buflen)
	// 	return -1;

	// /* Assure that the UDP header is within the buffer. */
	// if (ip_len + sizeof(*udp) > buflen)
	// 	return -1;

	udp_datagram_t *datagram = (udp_datagram_t *)(data->frame + sizeof(ethernet_frame_t) + sizeof(ipv4_packet_t));

	/*
	 * Compute UDP checksums, including the ``pseudo-header'', the
	 * UDP header and the data. If the UDP checksum field is zero,
	 * we're not supposed to do a checksum.
	 */
	uint32_t len = ntohs(datagram->length) - sizeof(udp_datagram_t);

	// if ((len < 0) || (len + data > buf + buflen)) {
	// 	// udp_packets_length_overflow++;
	// 	// if (udp_packets_length_checked > 4 &&
	// 	//     udp_packets_length_overflow != 0 &&
	// 	//     (udp_packets_length_checked /
	// 	//     udp_packets_length_overflow) < 2) {
	// 	// 	log_debug("%s: %d udp packets in %d too long - dropped",
	// 	// 	    log_procname, udp_packets_length_overflow,
	// 	// 	    udp_packets_length_checked);
	// 	// 	udp_packets_length_overflow =
	// 	// 	    udp_packets_length_checked = 0;
	// 	// }
	// 	return -1;
	// }

	// if (len + data != buf + buflen)
	// 	log_debug("%s: accepting packet with data after udp payload",
	// 	    log_procname);

	uint32_t usum = datagram->checksum;
	datagram->checksum = 0;
    //data = buf + ip_len + sizeof(*udp);
	uint32_t sum = wrapsum(
            checksum(
                (unsigned char *)datagram, sizeof(udp_datagram_t),
	            checksum(datagram->data, len, 
                        checksum((unsigned char *)&(packet->src_ip), 2 * IPv4_ADDRESS_LENGTH,
	                        IP_PROTOCOL_UDP + ntohs((uint32_t)datagram->length)
                        )
                )
            )
            );

	if (usum != 0 && usum != sum) {
		// udp_packets_bad_checksum++;
		// if (udp_packets_seen > 4 && udp_packets_bad_checksum != 0 &&
		//     (udp_packets_seen / udp_packets_bad_checksum) < 2) {
		// 	log_debug("%s: %d bad udp checksums in %d packets",
		// 	    log_procname, udp_packets_bad_checksum,
		// 	    udp_packets_seen);
		// 	udp_packets_seen = udp_packets_bad_checksum = 0;
		// }
		// return -1;
	}

	//memcpy(&from->sin_port, &udp->uh_sport, sizeof(udp->uh_sport));

	return sum; //ip_len + sizeof(*udp);

    // register uint32_t sum = 0;
    // udp_datagram_t *udphdrp = (udp_datagram_t *)(ipPayload);
    // uint16_t udpLen = __uint16_flip(udphdrp->length);
    // // the source ip
    // sum += ((*(uint32_t *)pIph->src_ip) >> 16) & 0xFFFF;
    // sum += ((uint32_t)pIph->src_ip) & 0xFFFF;
    // // the dest ip
    // sum += ((*(uint32_t *)pIph->dst_ip) >> 16) & 0xFFFF;
    // sum += ((uint32_t)pIph->dst_ip) & 0xFFFF;
    // // protocol and reserved: 17
    // sum += __uint16_flip((uint16_t)IP_PROTOCOL_UDP);
    // // the length
    // sum += udphdrp->length;

    // udphdrp->checksum = 0;

    // while (udpLen > 1)
    // {
    //     sum += *ipPayload++;
    //     udpLen -= 2;
    // }

    // if (udpLen > 0)
    //     sum += ((*ipPayload) & __uint16_flip(0xFF00));

    // while (sum >> 16)
    //     sum = (sum & 0xffff) + (sum >> 16);

    // sum = ~sum;
    // udphdrp->checksum = ((unsigned short)sum == 0x0000) ? 0xFFFF : (unsigned short)sum;
    
    // return udphdrp->checksum;
}

void __udp_flip_values(udp_datagram_t *datagram)
{
    datagram->src_port = __uint16_flip(datagram->src_port);
    datagram->dst_port = __uint16_flip(datagram->dst_port);
    datagram->length = __uint16_flip(datagram->length);
    datagram->checksum = __uint16_flip(datagram->checksum);
}