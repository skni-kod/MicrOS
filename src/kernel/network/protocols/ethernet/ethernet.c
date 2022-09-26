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

nic_data_t *ethernet_create_frame(net_device_t *device, uint16_t type, uint32_t data_size)
{
    nic_data_t *data = network_manager_get_transmitt_buffer(device);
    data->length = data_size + sizeof(ethernet_frame_t);
    memcpy(&((ethernet_frame_t *)(data->frame))->src, &device->interface->mac, sizeof(mac_addr_t));
    ((ethernet_frame_t *)(data->frame))->type = htons(type);
    data->device = device;
    return data;
}

uint32_t ethernet_send_frame(nic_data_t *data)
{
    switch (((ethernet_frame_t *)(data->frame))->type)
    {
    case htons(IPv4_PROTOCOL_TYPE):
    {
        arp_entry_t *mac = arp_request_entry(data->device,
                                             &((ipv4_packet_t *)(data->frame + sizeof(ethernet_frame_t)))->dst);
        if (mac)
        {
            if(!heap_kernel_verify_integrity()){
                int x = 1;
            }
            memcpy(&((ethernet_frame_t *)(data->frame))->dst, &mac->mac, sizeof(mac_addr_t));
            if(!heap_kernel_verify_integrity()){
                int x = 1;
            }
            // calculate FCS
            *(uint32_t *)(data->frame + data->length) = __crc32(data->frame, data->length);
            data->length++;
            return network_manager_send_data(data);
        }
    }
    case htons(ARP_PROTOCOL_TYPE):
    {
        *(uint32_t *)(data->frame + data->length) = __crc32(data->frame, data->length);
        data->length++;
        return network_manager_send_data(data);
    }
    break;
    };
}