#include "ipv4.h"

// TODO:
//  add function to track stack, so in ICMP be able to get header size etc
//  add function like ethernet_make_frame, ethernet_send_frame in IPv4,
//  rewrite defintions in protorols
//  eg: IPv4 PROTOCOL_TYPE
//  add ICMP enums

kvector *fragments;

void ipv4_process_packet(nic_data_t *data)
{
    ipv4_packet_t *packet = (ipv4_packet_t *)(data->frame + sizeof(ethernet_frame_t));

    if (packet->version != 4)
        return;

    //TODO: replying fragmented data
    // Assembly packet if it is fragmented
    for (uint16_t i = 0; i < fragments->count; i++)
    {
        nic_data_t *nic = (nic_data_t *)fragments->data[i];
        ipv4_packet_t *pkt = (ipv4_packet_t *)(nic->frame + sizeof(ethernet_frame_t));
        if (*(uint32_t *)pkt->src_ip == *(uint32_t *)packet->src_ip && pkt->id == packet->id)
        {
            // assume that data->frame buffer is big enought(driver sets its size to 1.5KB)
            uint16_t offset = (packet->offset << 8 | packet->offset2);
            memcpy(pkt->data+offset*8, packet->data, ntohs(packet->length) - packet->ihl);
            data->keep = false;
            if (packet->flags_mf == IPv4_FLAG_LAST_FRAGMENT)
            {
                data = nic;
                packet = (ipv4_packet_t *)(data->frame + sizeof(ethernet_frame_t));
                goto process;
            }
            return;
        }
    }

    if (packet->flags_mf == IPv4_FLAG_MORE_FRAGMENTS)
    {
        kvector_add(fragments, data);
        data->keep = true;
        return;
    }

process:
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
    packet->length = __uint16_flip(packet->length);
    packet->id = __uint16_flip(packet->id);
    packet->header_checksum = __uint16_flip(packet->header_checksum);
}

void ipv4_init()
{
    fragments = heap_kernel_alloc(sizeof(kvector), 0);
    kvector_init(fragments);
}

void ipv4_checksum(ipv4_packet_t *packet)
{
    packet->header_checksum = 0;
    packet->header_checksum = __ip_wrapsum(__ip_checksum((unsigned char *)packet, sizeof(ipv4_packet_t), 0));
}