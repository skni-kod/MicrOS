#include "socket.h"

socket_t socket_descriptors[SOCKET_DESCRIPTORS_COUNT] = {0};

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
                return socket_create_descriptor(AF_INET, SOCK_DGRAM, IPPROTO_TCP);
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
    if (socket)
        return socket->ops->bind(socket, addr, addrlen);

    return -1;
}

int k_recv(int s, void *buf, size_t len, int flags)
{
    socket_t *socket = socket_get_descriptor(s);
    if (socket)
        return socket->ops->recv(socket, buf, len, flags);

    return -1;
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
        return socket->ops->send(socket,buf,len,flags);

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

int socket_create_descriptor(int domain, int type, int protocol)
{
    socket_t *socket;

    for (uint8_t id = 1; id < SOCKET_DESCRIPTORS_COUNT; id++)
    {
        socket = &socket_descriptors[id];
        if (socket && socket->state == SS_FREE)
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

    return &socket_descriptors[socket];
}

int socket_read(socket_t *socket, void *data, size_t length, struct sockaddr *to)
{
    return socket->ops->read(socket, data, length, to);
}

int socket_write(socket_t *socket, void *data, size_t length, struct sockaddr *from)
{
    return socket->ops->write(socket, data, length, from);
}

socket_t *socket_descriptor_lookup(int domain, int type, int protocol, struct sockaddr *addr)
{
    socket_t *descriptor;

    // First look for descriptors with specified IP, then look for INADDR_ANY
    for (uint8_t id = 1; id < SOCKET_DESCRIPTORS_COUNT; id++)
    {
        descriptor = &socket_descriptors[id];
        if (descriptor &&
            descriptor->state == SS_CONNECTED &&
            descriptor->domain == domain &&
            descriptor->type == type &&
            descriptor->protocol == protocol)
        {
            switch (descriptor->protocol)
            {
            case IP_PROTOCOL_UDP:
            {
                udp_socket_t *sk = descriptor->sk;
                if (sk->local.sin_port == ((struct sockaddr_in *)addr)->sin_port &&
                        sk->local.sin_addr.address == ((struct sockaddr_in *)addr)->sin_addr.address ||
                    sk->local.sin_addr.address == INADDR_BROADCAST ||
                    sk->local.sin_addr.address >> 24 == 0x7F ||
                    sk->local.sin_addr.address == INADDR_ANY)
                    return descriptor;
            }
            break;
            default:
                break;
            }
        }
    }

    return 0;
}