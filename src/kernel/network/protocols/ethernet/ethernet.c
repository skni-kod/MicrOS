#include "ethernet.h"

uint32_t ethernet_process_frame(nic_data_t *data)
{
    ethernet_frame_t *frame = (ethernet_frame_t *)data->frame;

    char tmp[128];
    kernel_sprintf(tmp, "TYPE: %d",
                   frame->type);
    logger_log_info(tmp);

    switch (frame->type)
    {
    case htons(ARP_PROTOCOL_TYPE):
        return arp_process_packet(data);
    case htons(IPv4_PROTOCOL_TYPE):
        return ipv4_process_packet(data);
    default:
        return 0;
    };
}

nic_data_t *ethernet_create_frame(net_device_t *device, uint16_t type, uint32_t data_size)
{
    nic_data_t *data = network_manager_get_transmitt_buffer(device);
    data->length = data_size + sizeof(ethernet_frame_t);
    // Some NIC are strict, and does not allow to send frames below 64 bytes
    if (data->length < 64)
        data->length = 64;
    memcpy(&((ethernet_frame_t *)(data->frame))->src, &device->interface.mac, sizeof(mac_addr_t));
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
        ipv4_packet_t *packet = data->frame + sizeof(ethernet_frame_t);
        ipv4_addr_t net_addr = (ipv4_addr_t){.value =
                                                 data->device->interface.ipv4_address.value & data->device->interface.ipv4_netmask.value};
        ipv4_addr_t dst_net = (ipv4_addr_t){.value =
                                                dst_net.value = packet->dst.value & data->device->interface.ipv4_netmask.value};
        arp_entry_t *mac;

        if (dst_net.value != net_addr.value)
            mac = arp_request_entry(data->device, &data->device->interface.ipv4_gateway);
        else
            mac = arp_request_entry(data->device, &packet->dst);

        if (mac)
        {
            memcpy(&((ethernet_frame_t *)(data->frame))->dst, &mac->mac, sizeof(mac_addr_t));
            return network_manager_send_data(data);
        }
        else
            return 0;
    }
    break;
    case htons(ARP_PROTOCOL_TYPE):
        return network_manager_send_data(data);
        break;
    };
}