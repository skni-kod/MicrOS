/*
    @JakubPrzystasz
    Created: 06.02.2021
    Modified: 27.06.2022
*/

#include "network_manager.h"

#include "protocols/dhcp/dhcp.h"

kvector *net_devices;
bool (*drivers[])(net_device_t *device) = {rtl8139_init, virtio_nic_init};
uint8_t drivers_count = 2;

bool network_manager_init()
{

    net_devices = heap_kernel_alloc(sizeof(kvector), 0);
    kvector_init(net_devices);

    ipv4_init();

    // Initialize all NIC drivers
    for (uint8_t i = 0; i < drivers_count; i++)
    {
        net_device_t *dev = heap_kernel_alloc(sizeof(net_device_t), 0);
        if (__network_manager_set_net_device(dev) && drivers[i](dev))
        {
            // dev->interface->ipv4_address.oct_a = 192;
            // dev->interface->ipv4_address.oct_b = 168;
            // dev->interface->ipv4_address.oct_c = 1;
            // dev->interface->ipv4_address.oct_d = 165 + net_devices->count;
            kvector_add(net_devices, dev);
            __network_manager_print_device_info(dev);
        }
        else
        {
            heap_kernel_dealloc(dev->interface);
            heap_kernel_dealloc(dev);
        }
    }

    // when all devices are up, lets initalize them!
    for (uint32_t devices = 0; devices < net_devices->count; devices++)
    {
        net_device_t *dev = (net_device_t *)net_devices->data[devices];
        // setup receive and transmitt buffers
        dev->rx = kbuffer_init(dev->interface->mtu, NETWORK_MANAGER_BUFFER_SIZE);
        dev->tx = kbuffer_init(dev->interface->mtu, NETWORK_MANAGER_BUFFER_SIZE);
        // finally turn on communication
        dev->interface->mode.value = 0x3;
        dhcp_negotiate(dev->interface);
    }

    return true;
}

nic_data_t *network_manager_get_receive_buffer(net_device_t *device)
{
    if (device && device->interface->mode.receive)
    {

        nic_data_t *data = (nic_data_t *)kbuffer_get(device->rx, device->interface->mtu);
        data->length = device->interface->mtu;
        data->device = device;
        data->keep = false;
        return data;
    }
    else
        return 0;
}

nic_data_t *network_manager_get_transmitt_buffer(net_device_t *device)
{
    if (device)
    {
        nic_data_t *data = (nic_data_t *)kbuffer_get(device->tx, device->interface->mtu);
        data->length = device->interface->mtu;
        data->device = device;
        data->keep = false;
        return data;
    }
    else
        return 0;
}

uint32_t network_manager_send_data(nic_data_t *data)
{
    uint32_t ret = 0;
    if (data->device->interface->mode.send)
    {
        data->device->dpi.send(data);
        ++data->device->interface->frames_sent;
        data->device->interface->bytes_sent += data->length;
        ret = (uint32_t)data->length;
    }

    if (!data->keep)
        kbuffer_drop(data);

    return ret;
}

void network_manager_receive_data(nic_data_t *data)
{
    if (data->device->interface->mode.receive)
    {
        if (!ethernet_process_frame(data))
            ++data->device->interface->frames_dropped;
        ++data->device->interface->frames_received;
        data->device->interface->bytes_received += data->length;
    }
    if (!data->keep)
        kbuffer_drop(data);
}

net_device_t *network_manager_get_nic()
{
    if (net_devices == 0)
        return 0;

    if (net_devices->count == 0)
        return 0;

    return net_devices->data[0];
}

net_device_t *network_manager_get_nic_by_ipv4(ipv4_addr_t addr)
{
    // TODO: default route etc..
    return network_manager_get_nic();
}

void __network_manager_print_device_info(net_device_t *device)
{
    char logInfo[27] = "";

    logger_log_ok(device->device_name);

    kernel_sprintf(logInfo, "MAC: %02x:%02x:%02x:%02x:%02x:%02x",
                   device->interface->mac.octet_a,
                   device->interface->mac.octet_b,
                   device->interface->mac.octet_c,
                   device->interface->mac.octet_d,
                   device->interface->mac.octet_e,
                   device->interface->mac.octet_f);

    logger_log_info(logInfo);
}

static int8_t __network_manager_set_net_device(net_device_t *device)
{
    if (!device)
        return 0;
    memset(device, 0, sizeof(net_device_t));

    device->dpi.receive = &network_manager_receive_data;
    device->dpi.get_receive_buffer = &network_manager_get_receive_buffer;

    device->interface = heap_kernel_alloc(sizeof(net_interface_t), 0);
    if (!device->interface)
        return 0;

    memset(device->interface, 0, sizeof(net_interface_t));

    device->interface->arp_entries = heap_kernel_alloc(sizeof(kvector), 0);
    if (!device->interface->arp_entries)
        return 0;
    kvector_init(device->interface->arp_entries);

    // TODO: get mtu from driver
    device->interface->mtu = 1600;
    device->interface->ttl = 64;

    return 1;
}
