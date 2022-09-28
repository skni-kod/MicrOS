#include "socket.h"

socket_descriptor_t socket_descriptors[SOCKET_DESCRIPTORS] = {0};

int socket(int domain, int type, int protocol)
{
    switch (domain)
    {
    case AF_INET:
        break;
    default:
        return 0;
    }

    switch (type)
    {
    case SOCK_DGRAM:
    case SOCK_STREAM:
        break;
    default:
        return 0;
    }

    switch (protocol)
    {
    case IPPROTO_UDP:
    case IPPROTO_TCP:
        return socket_create_descriptor(domain, type, protocol);
    default:
        return 0;
    }
}

uint32_t recv(int s, void *buf, size_t len, int flags)
{
    socket_descriptor_t *descriptor = socket_get_descriptor(s);

    if (descriptor == NULL)
    {
        return 0;
    }

    if (descriptor->domain != AF_INET)
    {
        return 0;
    }

    switch (descriptor->type)
    {
    case SOCK_STREAM:
        break;
    default:
        return 0;
    }

    switch (descriptor->protocol)
    {
    case IPPROTO_TCP:
        break;
    default:
        return 0;
    }

    // return (uint32_t)socket_read(descriptor, from, buf, length);
}

uint32_t recvfrom(int s, void *buf, size_t length, int flags, struct sockaddr *from, socklen_t *fromlen)
{
    socket_descriptor_t *descriptor = socket_get_descriptor(s);

    if (descriptor == NULL)
    {
        return 0;
    }

    if (descriptor->domain != AF_INET)
    {
        return 0;
    }

    switch (descriptor->type)
    {
    case SOCK_DGRAM:
        break;
    default:
        return 0;
    }

    switch (descriptor->protocol)
    {
    case IPPROTO_UDP:
        break;
    default:
        return 0;
    }

    return (uint32_t)socket_read(descriptor, from, buf, length);
}

int bind(int s, struct sockaddr *my_addr, socklen_t addrlen)
{
    struct sockaddr_in *sin = (struct sockaddr_in *)my_addr;
    socket_descriptor_t *socket = socket_get_descriptor(s);

    if (socket)
    {
        struct sockaddr_in *s = (struct sockaddr_in *)my_addr;
        socket->port = s->sin_port;
    }
    else
        return -1;
}

uint32_t send(int s, const void *buf, size_t len, int flags)
{
    socket_descriptor_t *desc = socket_get_descriptor(s);

    net_device_t *device = network_manager_get_nic();

    switch (desc->type)
    {
    case SOCK_STREAM:
        switch (desc->protocol)
        {
        case IPPROTO_TCP:
        {
            // nic_data_t *data = udp_create_datagram(device, (ipv4_addr_t)((struct sockaddr_in *)to)->sin_addr.s_addr, ((struct sockaddr_in *)to)->sin_port, desc->port, len);
            // memcpy(data->frame + sizeof(ethernet_frame_t) + sizeof(ipv4_packet_t) + sizeof(udp_datagram_t), buf, len);
            // return udp_send_datagram(data);
        }
        default:
            return 0;
        }
    }

    return 0;
}

uint32_t sendto(int s, const void *buf, size_t len, int flags, const struct sockaddr *to, socklen_t tolen)
{
    socket_descriptor_t *desc = socket_get_descriptor(s);

    net_device_t *device = network_manager_get_nic();

    switch (desc->type)
    {
    case SOCK_DGRAM:
        switch (desc->protocol)
        {

        case IPPROTO_UDP:
        {
            nic_data_t *data = udp_create_datagram(device, (ipv4_addr_t)((struct sockaddr_in *)to)->sin_addr.s_addr, ((struct sockaddr_in *)to)->sin_port, desc->port, len);
            memcpy(data->frame + sizeof(ethernet_frame_t) + sizeof(ipv4_packet_t) + sizeof(udp_datagram_t), buf, len);
            return udp_send_datagram(data);
        }
        default:
            return 0;
        }

    case SOCK_RAW:
        switch (desc->protocol)
        {
        case IPPROTO_ICMP:
            return len;

        default:
            return 0;
        }
    }

    return 0;
}

int listen(int s, int backlog)
{
    socket_descriptor_t *desc = socket_get_descriptor(s);

    // if (backlog > 0 && backlog <= SOCKET_BACKLOG_MAX)
    //     desc->backlog = backlog;
    // else
    //     desc->backlog = SOCKET_BACKLOG_MAX;
    // desc->tcb = 0;
    // desc->tcb = heap_kernel_alloc(sizeof(tcb_t) * backlog, 0);

    // if (!desc->tcb)
    //     return -1;

    return 0;
}

int accept(int s, struct sockaddr *addr, socklen_t *addrlen)
{
    socket_descriptor_t *desc = socket_get_descriptor(s);

    // create and return tcb's index in server socket array (desc->tcb)
    return -1;
}

int socket_create_descriptor(int domain, int type, int protocol)
{
    socket_descriptor_t *descriptor;

    for (uint8_t id = 1; id < SOCKET_DESCRIPTORS; id++)
    {
        descriptor = &socket_descriptors[id];
        if (descriptor)
        {
            descriptor->used = true;
            descriptor->domain = domain;
            descriptor->type = type;
            descriptor->protocol = protocol;
            descriptor->port = SOCKET_BASE_PORT + id;
            descriptor->buffer = socket_init_buffer(descriptor->buffer, SOCKET_BUFFER_ENTRY_SIZE, SOCKET_BUFFER_ENTRY_COUNT);
            return id;
        }
    }

    return 0;
}

socket_buffer_t *socket_init_buffer(socket_buffer_t *buffer, size_t entry_size, uint32_t entry_count)
{
    buffer = heap_kernel_alloc(sizeof(socket_buffer_t) + (sizeof(socket_entry_t) + entry_size) * entry_count, 0);
    buffer->length = entry_count;
    buffer->entry_size = entry_size;
    buffer->read = 0;
    buffer->write = 0;
    return buffer;
}

socket_descriptor_t *socket_get_descriptor(uint32_t id)
{
    if (id >= SOCKET_DESCRIPTORS)
        return NULL;

    return socket_descriptors + id;
}

uint32_t socket_read(socket_descriptor_t *socket, struct sockaddr_in *addr, void *data, size_t length)
{
    if (socket->buffer->write != socket->buffer->read)
    {
        socket->buffer->read = socket->buffer->read + 1 & (socket->buffer->length - 1);
        socket_entry_t *entry = ((uint32_t)&socket->buffer->entries + ((size_t)(socket->buffer->read) * (socket->buffer->entry_size + (size_t)sizeof(socket_entry_t))));
        if (addr)
            memcpy(addr, &entry->addr, sizeof(struct sockaddr_in));
        memcpy(data, entry->data, entry->size);
        return entry->size;
    }
    return 0;
}

uint32_t socket_write(socket_descriptor_t *socket, struct sockaddr_in *addr, void *data, size_t length)
{
    if (socket->buffer->entry_size > length)
    {
        socket->buffer->write = socket->buffer->write + 1 & (socket->buffer->length - 1);
        socket_entry_t *entry = ((uint32_t) & (socket->buffer->entries) + ((size_t)(socket->buffer->write) * (socket->buffer->entry_size + (size_t)sizeof(socket_entry_t))));
        if (addr)
            memcpy(&entry->addr, addr, sizeof(struct sockaddr_in));
        memcpy(&entry->data, data, length);
        return entry->size = length;
    }
    else
        return 0;
}

socket_descriptor_t *socket_descriptor_lookup(int domain, int type, int protocol, struct sockaddr_in *addr)
{
    socket_descriptor_t *descriptor;

    // First look for descriptors with specified IP, then look for INADDR_ANY
    for (uint8_t id = 0; id < SOCKET_DESCRIPTORS; id++)
    {
        descriptor = &socket_descriptors[id];
        if (descriptor &&
            descriptor->used &&
            descriptor->domain == domain &&
            descriptor->type == type &&
            descriptor->protocol == protocol &&
            descriptor->port == addr->sin_port &&
            (descriptor->addr.sin_addr.s_addr == addr->sin_addr.s_addr ||
             descriptor->addr.sin_addr.s_addr == INADDR_BROADCAST ||
             descriptor->addr.sin_addr.s_addr >> 24 == 0x7F ||
             descriptor->addr.sin_addr.s_addr == INADDR_ANY))
            return descriptor;
    }

    return 0;
}