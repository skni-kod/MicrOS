#include "udp.h"

static struct proto_ops udp_interface = {
    .release = &socket_not_implemented,
    .bind = &udp_socket_bind,
    .connect = &udp_socket_connect,
    .socketpair = &socket_not_implemented,
    .accept = &socket_not_implemented,
    .getname = &socket_not_implemented,
    .poll = &socket_not_implemented,
    .ioctl = &socket_not_implemented,
    .listen = &socket_not_implemented,
    .shutdown = &socket_not_implemented,
    .sendmsg = &socket_not_implemented,
    .recvmsg = &socket_not_implemented,
    .send = &udp_socket_send,
    .sendto = &udp_socket_sendto,
    .recv = &udp_socket_recv,
    .recvfrom = &udp_socket_recvfrom,
};

uint32_t udp_process_datagram(nic_data_t *data)
{
    ipv4_packet_t *packet = ((ethernet_frame_t *)(data->frame))->data;
    udp_datagram_t *datagram = packet->data;

#ifndef TRUST_ME_BRO
    // verify checksum
    uint16_t checksum = datagram->checksum;
    if (checksum != udp_checksum(packet))
        return 0;
#endif

    struct sockaddr_in addr = {
        .sin_addr.value = packet->dst.value,
        .sin_port = datagram->dst_port};

    // first look for open socket, then forward there incoming data:
    socket_t *socket = __udp_get_socket(&addr);

    if (socket)
    {
        addr.sin_addr.value = packet->src.value;
        addr.sin_port = datagram->src_port;
        udp_socket_write(socket, datagram->data, ntohs(datagram->length) - sizeof(udp_datagram_t), &addr);
        return 1;
    }

    return 0;
}

uint16_t udp_checksum(ipv4_packet_t *packet)
{
    udp_datagram_t *datagram = (udp_datagram_t *)packet->data;

    uint32_t len = ntohs(datagram->length) - sizeof(udp_datagram_t);

    datagram->checksum = 0;

    uint32_t sum = __ip_wrapsum(
        __ip_checksum(
            (unsigned char *)datagram, sizeof(udp_datagram_t),
            __ip_checksum(datagram->data, len,
                          __ip_checksum((unsigned char *)&(packet->src), 2 * sizeof(ipv4_addr_t),
                                        IP_PROTOCOL_UDP + ntohs((uint32_t)datagram->length)))));
    return datagram->checksum = sum;
}

nic_data_t *udp_create_datagram(net_device_t *device, ipv4_addr_t dst_addr, uint16_t dst_port, uint16_t src_port, uint32_t data_size)
{
    nic_data_t *data = ipv4_create_packet(device, IP_PROTOCOL_UDP, dst_addr, data_size + sizeof(udp_datagram_t));
    ipv4_packet_t *packet = data->frame + sizeof(ethernet_frame_t);
    udp_datagram_t *datagram = &packet->data;
    datagram->dst_port = dst_port;
    datagram->src_port = src_port;
    datagram->length = htons((data_size + sizeof(udp_datagram_t)));
    return data;
}

static socket_t *__udp_get_socket(struct sockaddr_in *addr)
{
    socket_t *ret = 0;
    for (uint32_t id = 1; id < SOCKET_DESCRIPTORS_COUNT; id++)
    {
        struct socket *socket = socket_descriptors[id];

        if (socket &&
            socket->domain == AF_INET &&
            socket->type == SOCK_DGRAM &&
            socket->protocol == IP_PROTOCOL_UDP)
        {
            udp_socket_t *sk = socket->sk;
            if (sk->local.sin_port == addr->sin_port)
            {
                if (sk->local.sin_addr.value == addr->sin_addr.value)
                    // best match -- connection socket
                    return socket;
                else if (INADDR_BROADCAST == sk->local.sin_addr.value || INADDR_ANY == sk->local.sin_addr.value)
                    // listen socket found
                    ret = socket;
            }
        }
    }

    return ret;
}

uint32_t udp_send_datagram(nic_data_t *data)
{
    ipv4_packet_t *packet = data->frame + sizeof(ethernet_frame_t);
#ifndef TRUST_ME_BRO
    udp_checksum(packet);
#endif
    return ipv4_send_packet(data);
}

socket_t *udp_socket_init(socket_t *socket)
{
    static uint16_t port = SOCKET_BASE_PORT;
    socket->state = SS_CONNECTING;
    socket->sk = heap_kernel_alloc(sizeof(udp_socket_t), 0);
    udp_socket_t *sk = socket->sk;
    sk->buffer = socket_init_buffer(sk->buffer, SOCKET_BUFFER_ENTRY_SIZE, SOCKET_BUFFER_ENTRY_COUNT);
    sk->device = network_manager_get_nic();
    sk->local.sin_family = AF_INET;
    sk->local.sin_port = SOCKET_BASE_PORT + port++;
    sk->local.sin_addr.value = sk->device->interface->ipv4_address.value;

    socket->ops = heap_kernel_alloc(sizeof(struct proto_ops), 0);
    memcpy(socket->ops, &udp_interface, sizeof(struct proto_ops));

    return socket;
}

int udp_socket_bind(socket_t *socket, struct sockaddr *addr, int addrlen)
{
    udp_socket_t *sk = socket->sk;
    struct sockaddr_in *_addr = (struct sockaddr_in *)addr;
    sk->local.sin_port = _addr->sin_port;
    sk->local.sin_addr.value = _addr->sin_addr.value;
    return 0;
}

int udp_socket_recv(struct socket *socket, void *buf, size_t len, int flags)
{
    udp_socket_t *sk = socket->sk;
    if (sk->buffer->write != sk->buffer->read)
    {
        sk->buffer->read = sk->buffer->read + 1 & (sk->buffer->length - 1);
        socket_entry_t *entry = ((uint32_t)&sk->buffer->entries + ((size_t)(sk->buffer->read) * (sk->buffer->entry_size + (size_t)sizeof(socket_entry_t))));
        struct sockaddr_in *from = &sk->remote;
        if (from)
            memcpy(from, &entry->addr, sizeof(struct sockaddr_in));
        memcpy(buf, entry->data, entry->size);
        return entry->size;
    }

    return 0;
}

int udp_socket_recvfrom(struct socket *socket, void *buf, size_t len, int flags,
                        struct sockaddr *from, socklen_t *fromlen)
{
    udp_socket_t *sk = socket->sk;
    if (sk->buffer->write != sk->buffer->read)
    {
        sk->buffer->read = sk->buffer->read + 1 & (sk->buffer->length - 1);
        socket_entry_t *entry = ((uint32_t)&sk->buffer->entries + ((size_t)(sk->buffer->read) * (sk->buffer->entry_size + (size_t)sizeof(socket_entry_t))));
        if (from)
            memcpy(from, &entry->addr, sizeof(struct sockaddr_in));
        memcpy(buf, entry->data, entry->size);
        return entry->size;
    }

    return 0;
}

int udp_socket_send(struct socket *socket, void *buf, size_t len, int flags)
{
    udp_socket_t *sk = socket->sk;
    struct sockaddr_in *to = &sk->remote;
    nic_data_t *data = udp_create_datagram(sk->device, ((struct sockaddr_in *)to)->sin_addr, ((struct sockaddr_in *)to)->sin_port, sk->local.sin_port, len);
    memcpy(data->frame + sizeof(ethernet_frame_t) + sizeof(ipv4_packet_t) + sizeof(udp_datagram_t), buf, len);
    return udp_send_datagram(data);
}

int udp_socket_sendto(struct socket *socket, void *buf, size_t len, int flags,
                      struct sockaddr *to, socklen_t *tolen)
{
    udp_socket_t *sk = socket->sk;
    nic_data_t *data = udp_create_datagram(sk->device, ((struct sockaddr_in *)to)->sin_addr, ((struct sockaddr_in *)to)->sin_port, sk->local.sin_port, len);
    memcpy(data->frame + sizeof(ethernet_frame_t) + sizeof(ipv4_packet_t) + sizeof(udp_datagram_t), buf, len);
    return udp_send_datagram(data);
}

int udp_socket_connect(struct socket *socket, const struct sockaddr *serv_addr, socklen_t addrlen)
{
    udp_socket_t *sk = socket->sk;
    memcpy(&sk->remote, serv_addr, addrlen);
    socket->state = SS_CONNECTED;
}

int udp_socket_write(struct socket *socket, void *buf, size_t length, struct sockaddr *addr)
{
    udp_socket_t *sk = socket->sk;
    if (sk->buffer->entry_size > length)
    {
        sk->buffer->write = sk->buffer->write + 1 & (sk->buffer->length - 1);
        socket_entry_t *entry = ((uint32_t) & (sk->buffer->entries) + ((size_t)(sk->buffer->write) * (sk->buffer->entry_size + (size_t)sizeof(socket_entry_t))));
        if (addr)
            memcpy(&entry->addr, addr, sizeof(struct sockaddr_in));
        memcpy(&entry->data, buf, length);
        return entry->size = length;
    }
    else
        return 0;
}

int udp_socket_read(struct socket *socket, void *buf, size_t length, struct sockaddr *addr)
{
    udp_socket_t *sk = socket->sk;
    if (sk->buffer->entry_size > length)
    {
        sk->buffer->write = sk->buffer->write + 1 & (sk->buffer->length - 1);
        socket_entry_t *entry = ((uint32_t) & (sk->buffer->entries) + ((size_t)(sk->buffer->write) * (sk->buffer->entry_size + (size_t)sizeof(socket_entry_t))));
        if (addr)
            memcpy(addr, &entry->addr, sizeof(struct sockaddr_in));
        memcpy(buf, &entry->data, length);
        return entry->size = length;
    }
    else
        return 0;
}
