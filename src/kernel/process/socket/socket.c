#include "socket.h"

socket_t *socket_descriptors[SOCKET_DESCRIPTORS_COUNT] = {0};

static uint32_t timestamp = 0;

int k_socket(int domain, int type, int protocol)
{
    switch (domain)
    {
    case AF_INET:
    {
        switch (type)
        {
        case SOCK_DGRAM:
        {
            switch (protocol)
            {
            default:
                return socket_create_descriptor(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
            }
        }
        case SOCK_STREAM:
        {
            switch (protocol)
            {
            default:
                return socket_create_descriptor(AF_INET, SOCK_STREAM, IPPROTO_TCP);
            }
        }
        break;
        default:
            return 0;
        }
    }
    break;
    default:
        return 0;
    }
}

int k_bind(int s, struct sockaddr *addr, int addrlen)
{
    socket_t *socket = socket_get_descriptor(s);
    if (socket && SS_FREE != socket->state)
        return socket->ops->bind(socket, addr, addrlen);

    return -1;
}

int k_recv(int s, void *buf, size_t len, int flags)
{
    socket_t *socket = socket_get_descriptor(s);
    if (socket)
        return socket->ops->recv(socket, buf, len, flags);

    return 0;
}

int k_recvfrom(int s, void *buf, size_t length, int flags, struct sockaddr *from, int *fromlen)
{
    socket_t *socket = socket_get_descriptor(s);
    if (socket)
        return socket->ops->recvfrom(socket, buf, length, flags, from, fromlen);

    return -1;
}

int k_send(int s, const void *buf, size_t len, int flags)
{
    socket_t *socket = socket_get_descriptor(s);
    if (socket)
        return socket->ops->send(socket, buf, len, flags);

    return -1;
}

int k_sendto(int s, const void *buf, size_t len, int flags, const struct sockaddr *to, int tolen)
{
    socket_t *socket = socket_get_descriptor(s);
    if (socket)
        return socket->ops->sendto(socket, buf, len, flags, to, tolen);

    return -1;
}

int k_listen(int s, int backlog)
{
    socket_t *socket = socket_get_descriptor(s);
    if (socket)
        return socket->ops->listen(socket, backlog);

    return -1;
}

int k_accept(int s, struct sockaddr *addr, int *addrlen)
{
    socket_t *socket = socket_get_descriptor(s);
    if (socket)
        return socket->ops->accept(socket, addr, addrlen);

    return -1;
}

int k_connect(int s, struct sockaddr *addr, int *addrlen)
{
    socket_t *socket = socket_get_descriptor(s);
    if (socket)
        return socket->ops->connect(socket, addr, addrlen);

    return -1;
}

klist_t *socket_add_entry(klist_t *buffer, ipv4_packet_t *packet)
{
    switch (packet->protocol)
    {
    case IP_PROTOCOL_TCP:
    {
        uint32_t data_size = TCP_DATA_SIZE(packet);
        socket_entry_t *entry = heap_kernel_alloc(sizeof(socket_entry_t) + data_size, 0);
        entry->size = data_size;
        entry->timestamp = timestamp++;
        struct sockaddr_in *address = &(entry->addr);
        address->sin_family = AF_INET;
        address->sin_port = ((tcp_segment_t *)(packet->data))->src_port;
        address->sin_addr.address = packet->src.address;
        memcpy((*entry).data, (void *)TCP_DATA_PTR((*packet).data), data_size);
        return klist_add(buffer, (void *)entry);
    }
    default:
        break;
    }
}

void socket_not_implemented()
{
    logger_log_warning("Function not implemented");
}

int socket_create_descriptor(int domain, int type, int protocol)
{
    for (uint32_t id = 1; id < SOCKET_DESCRIPTORS_COUNT; id++)
    {
        struct socket *socket = socket_descriptors[id];
        // descriptor is empty
        if (!socket)
        {
            socket = socket_descriptors[id] = heap_kernel_alloc(sizeof(struct socket), 0);
            socket->state = SS_FREE;
        }

        if (socket->state == SS_FREE)
        {
            socket->domain = domain;
            socket->type = type;
            socket->protocol = protocol;
            switch (domain)
            {
            case AF_INET:
                network_init_socket(socket);
                break;
            };
            return id;
        }
    }

    return 0;
}

int socket_add_descriptor(struct socket *_socket)
{
    for (uint32_t id = 1; id < SOCKET_DESCRIPTORS_COUNT; id++)
    {
        struct socket *socket = socket_descriptors[id];
        if (!socket)
        {
            socket_descriptors[id] = _socket;
            return id;
        }
        if (SS_FREE == socket->state)
        {
            heap_kernel_dealloc(socket);
            socket_descriptors[id] = _socket;
            return id;
        }
    }
    return -1;
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

socket_t *socket_get_descriptor(int socket)
{
    if (socket >= SOCKET_DESCRIPTORS_COUNT || socket < 0)
        return NULL;

    return socket_descriptors[socket];
}

socket_t *socket_descriptor_lookup(int domain, int type, int protocol, struct sockaddr *addr)
{
    // First look for descriptors with specified IP, then look for INADDR_ANY
    for (uint32_t id = 1; id < SOCKET_DESCRIPTORS_COUNT; id++)
    {
        struct socket *socket = socket_descriptors[id];

        if (socket &&
            socket->state == SS_CONNECTED &&
            socket->domain == domain &&
            socket->type == type &&
            socket->protocol == protocol)
        {
            switch (socket->protocol)
            {
            case IP_PROTOCOL_UDP:
            {
                udp_socket_t *sk = socket->sk;
                if (sk->local.sin_port == ((struct sockaddr_in *)addr)->sin_port &&
                        sk->local.sin_addr.address == ((struct sockaddr_in *)addr)->sin_addr.address ||
                    INADDR_BROADCAST == sk->local.sin_addr.address ||
                    0x7F == sk->local.sin_addr.address >> 24 ||
                    INADDR_ANY == sk->local.sin_addr.address)
                    return socket;
            }
            case IP_PROTOCOL_TCP:
            {
                tcp_socket_t *sk = socket->sk;
                if (sk->local.sin_port == ((struct sockaddr_in *)addr)->sin_port &&
                        sk->local.sin_addr.address == ((struct sockaddr_in *)addr)->sin_addr.address ||
                    INADDR_BROADCAST == sk->local.sin_addr.address ||
                    0x7F == sk->local.sin_addr.address >> 24 ||
                    INADDR_ANY == sk->local.sin_addr.address)
                    return socket;
            }
            break;
            default:
                break;
            }
        }
    }

    return 0;
}