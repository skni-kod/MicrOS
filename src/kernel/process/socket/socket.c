#include "socket.h"

socket_descriptor_t socket_descriptors[SOCKET_DESCRIPTORS] = {0};

int socket(int domain, int type, int protocol)
{
    if (domain != AF_INET)
        return 0;

    if (type != SOCK_DGRAM)
        return 0;

    switch (protocol)
    {
    case IPPROTO_IP:
    case IPPROTO_ICMP:
    case IPPROTO_UDP:
    case IPPROTO_TCP:
        return socket_create_descriptor(domain, type, protocol);
    default:
        return 0;
    }
}

ssize_t recv(int s, void *buf, size_t len, int flags)
{
}

ssize_t recvfrom(int s, void *buf, size_t length, int flags, struct sockaddr *from, socklen_t *fromlen)
{
    socket_descriptor_t *descriptor = get_descriptor(s);

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
    case SOCK_RAW:
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

    memcpy(from, &descriptor->addr, descriptor->addr_len);
    *fromlen = descriptor->addr_len;

    return socket_entry_read(descriptor->entry, buf, length);
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

ssize_t sendto(int s, const void *buf, size_t len, int flags, const struct sockaddr *to, socklen_t tolen)
{
    socket_descriptor_t *desc = socket_get_descriptor(s);

    memcpy(&desc->addr, to, tolen);
    desc->addr_len = tolen;

    net_interface_t *interface = network_manager_get_nic();

    switch (desc->type)
    {
    case SOCK_DGRAM:
        switch (desc->protocol)
        {
            // case IPPROTO_IP:
            //     ipv4_send_packet(interface,
            //                      (struct sockaddr_in *)&desc->addr,
            //                      IPV4_PROTO_ICMP,
            //                      0,
            //                      (uint8_t *)buf,
            //                      len);
            //     return len;

        case IPPROTO_UDP:
            // udp_send_packet(interface,
            //                 desc->port,
            //                 interface->mac,
            //                 (struct sockaddr_in *)&desc->addr,
            //                 (uint8_t *)buf,
            //                 len);
            {
                // ethernet_frame_t *frame = ethernet_create_frame(
                //     &data->device->interface->mac,
                //     &((ethernet_frame_t *)data->frame)->src,
                //     IPv4_PROTOCOL_TYPE,
                //     htons(packet->length));

                // ipv4_packet_t *ip_reply = (ipv4_packet_t *)frame->data;
                // {
                //     memcpy(ip_reply, packet, sizeof(ipv4_packet_t));
                //     packet->flags_mf = IPv4_FLAG_DONT_FRAGMENT;
                //     memcpy(&ip_reply->dst, &packet->src, IPv4_ADDRESS_LENGTH);
                //     memcpy(&ip_reply->src, &packet->dst, IPv4_ADDRESS_LENGTH);
                // }

                // {
                //     udp_datagram_t *reply = (udp_datagram_t *)(frame->data + sizeof(ipv4_packet_t));
                //     reply->dst_port = datagram->src_port;
                //     reply->src_port = datagram->dst_port;
                //     reply->length = datagram->length;
                //     memcpy(reply->data, datagram->data, htons(packet->length) - (sizeof(ipv4_packet_t) + sizeof(udp_datagram_t)));
                // }

                // udp_checksum(ip_reply);
                // ethernet_send_frame(data->device, htons(packet->length), frame);
            }
            return len;

        default:
            // Indicate that something went wrong, even though the type and
            // protocol are supported.
            return 0;
        }

    case SOCK_RAW:
        switch (desc->protocol)
        {
        case IPPROTO_ICMP:
            // ipv4_send_packet(interface,
            //                  (struct sockaddr_in *)&desc->addr,
            //                  IPV4_PROTO_ICMP,
            //                  0,
            //                  (uint8_t *)buf,
            //                  len);
            return len;

        default:
            // Indicate that something went wrong, even though the type and
            // protocol are supported.
            return 0;
        }
    }

    return 0;
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
            descriptor->entry = heap_kernel_alloc(sizeof(socket_entry_t) + SOCKET_BUFFER_SIZE, 0);
            memset(descriptor->entry, 0, sizeof(socket_entry_t) + SOCKET_BUFFER_SIZE);
            descriptor->entry->length = SOCKET_BUFFER_SIZE;
            return id;
        }
    }

    return 0;
}

socket_descriptor_t *socket_get_descriptor(uint32_t id)
{
    if (id >= SOCKET_DESCRIPTORS)
        return NULL;

    return socket_descriptors + id;
}

size_t socket_entry_read(socket_entry_t *entry, void *buffer, size_t length)
{
    size_t ptr = 0;

    // read ptr follows write pointer in round buffer
    while (entry->read != entry->write)
    {
        // input buffer is full, do not copy data there!
        if (ptr >= length)
            return ptr;

        *(uint8_t *)(buffer + ptr++) = entry->buffer[entry->read++];

        if (entry->write == SOCKET_BUFFER_SIZE)
            entry->write = 0;
    }

    return ptr;
}

size_t socket_entry_write(socket_entry_t *entry, void *buffer, size_t length)
{
    size_t ptr = 0;

    // read ptr follows write pointer in round buffer
    while (true)
    {
        // input buffer is full, do not copy data there!
        if (ptr >= length)
            return ptr;

        entry->buffer[entry->write++] = *(uint8_t *)(buffer + ptr++);

        if (entry->write == SOCKET_BUFFER_SIZE)
            entry->write = 0;
    }

    return ptr;
}

socket_descriptor_t *socket_descriptor_lookup(int domain, int type, int protocol, uint16_t port)
{
    socket_descriptor_t *descriptor;

    for (uint8_t id = 0; id < SOCKET_DESCRIPTORS; id++)
    {
        descriptor = &socket_descriptors[id];
        if (descriptor &&
            descriptor->used &&
            descriptor->domain == domain &&
            descriptor->type == type &&
            descriptor->protocol == protocol &&
            descriptor->port == port)
            return descriptor;
    }

    return 0;
}