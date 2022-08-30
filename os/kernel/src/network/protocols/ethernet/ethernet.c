#include "ethernet.h"

void ethernet_process_frame(nic_data_t *data)
{
    ethernet_frame_t frame = *(ethernet_frame_t *)data->frame;
    frame.type = __uint16_flip(frame.type);

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

ethernet_frame_t *ethernet_make_frame(uint8_t *src_hw, uint8_t *dst_hw, uint16_t type, uint32_t payload_size)
{
    ethernet_frame_t *frame = heap_kernel_alloc(sizeof(ethernet_frame_t) + payload_size + ETHERNET_FCS_LENGTH, 0);

    memcpy(frame->dst_mac_addr, dst_hw, MAC_ADDRESS_SIZE);
    memcpy(frame->src_mac_addr, src_hw, MAC_ADDRESS_SIZE);

    frame->type = __uint16_flip(type);

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
    // send data
    network_manager_send_data(data);
}