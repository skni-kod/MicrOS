#include "ipv4.h"

void ipv4_process_packet(net_packet_t *_packet)
{
    /*
    void *data_ptr = _packet->packet_data + sizeof(ethernet_frame_t) - sizeof(uint8_t *);
    ipv4_packet_t *packet = (ipv4_packet_t*)data_ptr;
    __ipv4_flip_values(packet);

    // TOS
    // https://datatracker.ietf.org/doc/html/rfc2474
    // https://www.iana.org/assignments/dscp-registry/dscp-registry.xhtml
    
    switch (packet->protocol)
    {
    case 1:
        // ICMP
        {
            icmp_header *header = (icmp_header *)packet->data;
            if(header->type == 8){
                //echo request
                icmp_header *_icmp_reply = heap_kernel_alloc(sizeof(icmp_header) + 4,0);
                ipv4_packet_t *reply = heap_kernel_alloc(sizeof(ipv4_packet_t),0);
                net_packet_t *_reply = heap_kernel_alloc(_packet->packet_length,0);
                _reply->type = IPv4_PROTOCOL_TYPE;
                memcpy(_reply->dst_mac_addr,((ethernet_frame_t*)_packet->packet_data)->src_mac_addr,MAC_ADDRESS_SIZE);
                memcpy(_reply->src_mac_addr,_packet->device_mac,MAC_ADDRESS_SIZE);


                _icmp_reply->type = 0;
                _icmp_reply->code = 0;
                _icmp_reply->checksum = 0;
                _icmp_reply->checksum = ~(_icmp_reply->type | _icmp_reply->code | _icmp_reply->checksum);

                memcpy(reply,packet,sizeof(ipv4_packet_t));
                memcpy(reply->dst_ip,packet->src_ip,IPv4_ADDRESS_LENGTH);
                memcpy(reply->src_ip,packet->dst_ip,IPv4_ADDRESS_LENGTH);
                memcpy(reply->data,_icmp_reply,sizeof(icmp_header) + 4);
                network_manager_send_packet(_reply);
            }
        }
        break;
    case 17:;
        // UDP
        break;

    default:;
        break;
    }
    */
}

void __ipv4_flip_values(ipv4_packet_t *packet)
{
    *(packet->version_ihl_ptr) = __uint8_flip(*(packet->version_ihl_ptr));

    packet->length = __uint16_flip(packet->length);
    packet->id = __uint16_flip(packet->id);

    // FLAGS AND FRAGMENT OFFSET
    uint16_t tmp = (packet->fragment_offset_high << 8);
    tmp |= packet->fragment_offset_low;
    tmp |= (packet->flags << 13);
    memcpy(packet->flags_fragment_ptr, &tmp, sizeof(uint16_t));

    packet->header_checksum = __uint16_flip(packet->header_checksum);
}