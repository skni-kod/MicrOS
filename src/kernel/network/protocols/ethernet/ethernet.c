#include "ethernet.h"

void ethernet_process_frame(nic_data_t *data)
{
    ethernet_frame_t frame = *(ethernet_frame_t *)data->frame;

    switch (frame.type)
    {
    case htons(ARP_PROTOCOL_TYPE):
        arp_process_packet(data);
        break;
    case htons(IPv4_PROTOCOL_TYPE):
        ipv4_process_packet(data);
        break;
    };
}

nic_data_t *ethernet_create_frame(net_device_t *device, uint16_t type, uint32_t payload_size)
{
    nic_data_t *data = network_manager_get_transmitt_buffer(device);
    data->length = payload_size + sizeof(ethernet_frame_t);
    memcpy(&((ethernet_frame_t*)(data->frame))->src, &device->interface->mac, sizeof(mac_addr_t));
    ((ethernet_frame_t*)(data->frame))->type = htons(type);
    return data;
}

void ethernet_send_frame(nic_data_t *data, ipv4_addr_t *dst_addr)
{
    //TODO:
    // update frame dst mac addr
    switch(data->protocol){
        case arp:

            break;
        case ipv4:
            memcpy(&((ethernet_frame_t*)(data->frame))->dst, arp_get_entry(data->device, dst_addr), sizeof(mac_addr_t));
            break;
    };
    // calculate FCS
    *(uint32_t *)(data->frame + data->length) = __crc32(data->frame, data->length);
    network_manager_send_data(data);
}