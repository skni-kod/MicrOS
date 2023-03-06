/*
    @JakubPrzystasz
    Created: 06.02.2021
    Modified: 27.06.2022
*/
#include "network_manager.h"

kvector *net_devices;

nic_driver_init drivers[] = {rtl8139_probe /*,rtl8169_probe*/};

bool network_manager_init()
{
    net_devices = heap_kernel_alloc(sizeof(kvector), 0);
    kvector_init(net_devices);

    __crc32_build();
    ipv4_init();

    // Initialize all NIC drivers
    for (uint8_t i = 0; i < (sizeof(drivers) / sizeof(nic_driver_init)); i++)
        (drivers[i])(&network_manager_get_device);

    // when all devices are up, lets initalize them!
    for (uint32_t devices = 0; devices < net_devices->count; devices++)
    {
        net_device_t *dev = (net_device_t *)*(net_devices->data + devices);
        // setup receive and transmitt buffers
        dev->rx = kbuffer_init(dev->interface.mtu, NETWORK_MANAGER_BUFFER_SIZE);
        dev->tx = kbuffer_init(dev->interface.mtu, NETWORK_MANAGER_BUFFER_SIZE);
        // finally turn on communication
        dev->interface.mode = (net_mode_t){.receive = 1, .send = 1};

        dhcp_negotiate(&dev->interface);

        network_manager_print_device_info(dev);

        // if (dhcp_negotiate(dev->interface))
        //{
        // set static IP
        // dev->interface.ipv4_address = (ipv4_addr_t){
        //     .oct_a = 192,
        //     .oct_b = 168,
        //     .oct_c = 78,
        //     .oct_d = 20};

        // dev->interface.ipv4_dns = (ipv4_addr_t){
        //     .oct_a = 1,
        //     .oct_b = 1,
        //     .oct_c = 1,
        //     .oct_d = 1};

        // dev->interface.ipv4_netmask = (ipv4_addr_t){
        //     .oct_a = 255,
        //     .oct_b = 255,
        //     .oct_c = 255,
        //     .oct_d = 0};

        // dev->interface.ipv4_gateway = (ipv4_addr_t){
        //     .oct_a = 192,
        //     .oct_b = 168,
        //     .oct_c = 78,
        //     .oct_d = 1};
        //}
    }

    return true;
}

nic_data_t *network_manager_get_receive_buffer(net_device_t *device)
{
    if (device && device->interface.mode.receive)
    {

        nic_data_t *data = (nic_data_t *)kbuffer_get(device->rx, device->interface.mtu);
        data->length = device->interface.mtu;
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
        nic_data_t *data = (nic_data_t *)kbuffer_get(device->tx, device->interface.mtu);
        data->length = device->interface.mtu;
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
    if (data->device->interface.mode.send)
    {
        data->device->dpi.send(data);
        ++data->device->interface.frames_sent;
        data->device->interface.bytes_sent += data->length;
        ret = (uint32_t)data->length;
    }

    if (!data->keep)
        kbuffer_drop(data);

    return ret;
}

void network_manager_receive_data(nic_data_t *data)
{
    if (data->device->interface.mode.receive)
    {
        if (!ethernet_process_frame(data))
            ++data->device->interface.frames_dropped;
        ++data->device->interface.frames_received;
        data->device->interface.bytes_received += data->length;
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

net_device_t *network_manager_get_device(void)
{
    net_device_t *dev = 0;
    dev = heap_kernel_alloc(sizeof(net_device_t), 0);
    if (!network_manager_set_net_device(dev))
    {
        kvector_add(net_devices, dev);
    }
    else
    {
        return 0;
    }
    return dev;
}

net_device_t *network_manager_get_nic_by_ipv4(ipv4_addr_t addr)
{
    // TODO: default route etc..
    return network_manager_get_nic();
}

kvector *network_manager_get_devices()
{
    return net_devices;
}

static void network_manager_print_device_info(net_device_t *device)
{
    char logInfo[256];
    kernel_sprintf(logInfo, "%s MAC: %02x:%02x:%02x:%02x:%02x:%02x IP: %01d.%01d.%01d.%01d",
                   device->interface.name,
                   device->interface.mac.octet_a,
                   device->interface.mac.octet_b,
                   device->interface.mac.octet_c,
                   device->interface.mac.octet_d,
                   device->interface.mac.octet_e,
                   device->interface.mac.octet_f,
                   device->interface.ipv4_address.oct_a,
                   device->interface.ipv4_address.oct_b,
                   device->interface.ipv4_address.oct_c,
                   device->interface.ipv4_address.oct_d);

    logger_log_info(logInfo);
}

static uint32_t network_manager_set_net_device(net_device_t *device)
{
    if (!device)
        return 1;
    memset(device, 0, sizeof(net_device_t));

    device->dpi.receive = &network_manager_receive_data;
    device->dpi.get_receive_buffer = &network_manager_get_receive_buffer;

    // TODO:
    device->dpi.set_mode = 0xDEADDEAD;

    device->interface.arp_entries = heap_kernel_alloc(sizeof(kvector), 0);
    kvector_init(device->interface.arp_entries);

    device->interface.mtu = 1500;
    device->interface.ttl = 64;

    return 0;
}
