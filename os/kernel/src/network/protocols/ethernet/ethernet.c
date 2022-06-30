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
        //ipv4_process_packet(packet);
        break;
    case IPv6_PROTOCOL_TYPE:
        break;
    };
}

// bool network_manager_send_ethernet_frame(ethernet_frame_t *frame, uint32_t data_len)
// {
//     void *data = heap_kernel_alloc(sizeof(ethernet_frame_t) + data_len, 0);

//     nic_data_t data;
//     data.packet_length = data_len + sizeof(ethernet_frame_t) - sizeof(void *);
//     memcpy(data.device_mac, frame->src_mac_addr, MAC_ADDRESS_SIZE);
//     data.packet_data = data;

//     uint8_t offset = sizeof(ethernet_frame_t) - sizeof(void *);
//     memcpy(data, frame, offset);
//     memcpy(data + offset, frame->data, data_len);

//     network_manager_send_packet(&packet);
//     heap_kernel_dealloc(data);
//     return true;
// }

ethernet_frame_t *ethernet_make_frame(uint8_t *src_hw, uint8_t *dst_hw, uint16_t type, uint32_t payload_size)
{
    ethernet_frame_t *frame = heap_kernel_alloc(sizeof(ethernet_frame_t) + payload_size + ETHERNET_FCS_LENGTH, 0);
    memcpy(frame->dst_mac_addr, dst_hw, MAC_ADDRESS_SIZE);
    memcpy(frame->src_mac_addr, src_hw, MAC_ADDRESS_SIZE);

    frame->type = __uint16_flip(type);

    return frame;
}


void ethernet_send_frame(net_device_t *device, uint32_t payload_size, uint8_t* frame){
    nic_data_t *data = heap_kernel_alloc(sizeof(nic_data_t),0);
    data->device = device;
    data->length = payload_size + sizeof(ethernet_frame_t) + ETHERNET_FCS_LENGTH;
    //TODO: CALCULATE CRC AT THE END OF FRAME
    data->frame = frame;

    //send data
    network_manager_send_data(data);
}