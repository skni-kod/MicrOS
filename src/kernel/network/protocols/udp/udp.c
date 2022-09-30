#include "udp.h"

static struct proto_ops udp_interface = {
    .bind = &udp_socket_bind,
    .recvfrom = &udp_socket_recvfrom,
    .sendto = &udp_socket_sendto,
    .write = &udp_socket_write,
    .read = &udp_socket_read,
    .connect = &udp_socket_connect,
    .send = &udp_socket_send,
    .recv = &udp_socket_recv,
}
;

uint32_t udp_process_datagram(nic_data_t *data)
{
    ipv4_packet_t *packet = (ipv4_packet_t *)(data->frame + sizeof(ethernet_frame_t));
    udp_datagram_t *datagram = packet->data;

#ifndef TRUST_ME_BRO
    // verify checksum
    uint16_t checksum = datagram->checksum;
    if (checksum != udp_checksum(packet))
        return 0;
#endif

    struct sockaddr_in addr = {
        .sin_addr.address = packet->dst.address,
        .sin_port = datagram->dst_port};

    // first look for open socket, then forward there incoming data:
    socket_t *socket = socket_descriptor_lookup(AF_INET, SOCK_DGRAM, IPPROTO_UDP, &addr);

    if (socket)
    {
        addr.sin_addr.address = packet->src.address;
        addr.sin_port = datagram->src_port;
        socket_write(socket, datagram->data, ntohs(datagram->length) - sizeof(udp_datagram_t), &addr);
        return 1;
    }
    
    return 0;
}

uint16_t udp_checksum(ipv4_packet_t *packet)
{
    /* Check the IP header checksum - it should be zero. */
    // if (__ip_wrapsum(__ip_checksum((unsigned char *)packet, sizeof(ipv4_packet_t), 0)) != 0)
    //     return -1;

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
    sk->local.sin_addr.address = sk->device->interface->ipv4.address;

    socket->ops = heap_kernel_alloc(sizeof(struct proto_ops), 0);
    memcpy(socket->ops, &udp_interface, sizeof(struct proto_ops));

    return socket;
}

int udp_socket_bind(socket_t *socket, struct sockaddr *addr, int addrlen)
{
    udp_socket_t *sk = socket->sk;
    struct sockaddr_in *_addr = (struct sockaddr_in *)addr;
    sk->local.sin_port = _addr->sin_port;
    sk->local.sin_addr.address = _addr->sin_addr.address;
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
