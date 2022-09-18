#include "ethernet.h"

void ethernet_process_frame(nic_data_t *data)
{
    ethernet_frame_t frame = *(ethernet_frame_t *)data->frame;
    frame.type = htons(frame.type);

    switch (frame.type)
    {
    case ARP_PROTOCOL_TYPE:
        arp_process_packet(data);
        break;
    case IPv4_PROTOCOL_TYPE:
        ipv4_process_packet(data);
        break;
    };
}

ethernet_frame_t *ethernet_create_frame(mac_addr_t *src, mac_addr_t *dst, uint16_t type, uint32_t payload_size)
{
    ethernet_frame_t *frame = heap_kernel_alloc(sizeof(ethernet_frame_t) + payload_size + ETHERNET_FCS_LENGTH, 0);

    memcpy(&frame->dst, dst, sizeof(mac_addr_t));
    memcpy(&frame->src, src, sizeof(mac_addr_t));
    frame->type = htons(type);

    return frame;
}

void ethernet_send_frame(net_device_t *device, uint32_t payload_size, ethernet_frame_t *frame)
{
    nic_data_t *data = heap_kernel_alloc(sizeof(nic_data_t), 0);
    data->device = device;
    data->length = payload_size + sizeof(ethernet_frame_t) + ETHERNET_FCS_LENGTH;
    data->frame = (uint8_t *)frame;
    //FCS
    *(uint32_t *)(data->frame + payload_size + sizeof(ethernet_frame_t)) = __crc32(data->frame, payload_size + sizeof(ethernet_frame_t));
    network_manager_send_data(data);
}