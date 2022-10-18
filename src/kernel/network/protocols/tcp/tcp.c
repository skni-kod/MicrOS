#include "tcp.h"

static struct proto_ops tcp_interface = {
    .release = &socket_not_implemented,
    .bind = &tcp_socket_bind,
    .connect = &tcp_socket_connect,
    .socketpair = &socket_not_implemented,
    .accept = &tcp_socket_accept,
    .getname = &socket_not_implemented,
    .poll = &socket_not_implemented,
    .ioctl = &socket_not_implemented,
    .listen = &tcp_socket_listen,
    .shutdown = &socket_not_implemented,
    .sendmsg = &socket_not_implemented,
    .recvmsg = &socket_not_implemented,
    .send = &tcp_socket_send,
    .sendto = &socket_not_implemented,
    .recv = &tcp_socket_recv,
    .recvfrom = &socket_not_implemented,
};

uint32_t tcp_process_segment(nic_data_t *data)
{
    ipv4_packet_t *packet = (ipv4_packet_t *)(data->frame + sizeof(ethernet_frame_t));
    tcp_segment_t *segment = packet->data;

#ifndef TRUST_ME_BRO
    // verify checksum
    uint16_t checksum = segment->checksum;
    tcp_checksum(packet);
    if (checksum != tcp_checksum(packet))
        return 0;
#endif

    struct sockaddr_in addr = {
        .sin_addr.address = packet->dst.address,
        .sin_port = segment->dst_port};

    // first look for open socket, so forward there incoming data:
    socket_t *socket = __tcp_get_socket(&addr);

    static uint16_t timestamp = 1;

    if (socket)
    {
        addr.sin_addr.address = packet->src.address;
        addr.sin_port = segment->src_port;
        tcp_socket_t *sk = socket->sk;

        // Here we have all of incoming segments, to our server eg. 192.168.1.165:80
        // we need to route segments, it we have active connections on that address,
        // or if address does not match, lets try to create new one
        switch (sk->state)
        {
        case TCP_LISTEN:
            // if incoming segment has syn flag
            if (segment->syn)
            {
                for (int i = 0; i < sk->backlog; i++)
                {
                    struct socket *sock = sk->connections + i;
                    if (SS_FREE == sock->state)
                    {
                        // new client attempts to connect
                        tcp_socket_t *con = (tcp_socket_t *)(sock->sk);
                        sock->state = SS_CONNECTING;
                        sock->domain = AF_INET;
                        sock->type = SOCK_STREAM;
                        sock->protocol = IP_PROTOCOL_TCP;
                        con->state = TCP_SYN_RECV;
                        memcpy(&con->local, &sk->local, sizeof(struct sockaddr_in));
                        con->remote.sin_family = AF_INET;
                        con->remote.sin_addr.address = packet->src.address;
                        con->remote.sin_port = segment->src_port;
                        con->rx = klist_init(con->rx);
                        con->tx = klist_init(con->tx);

                        void *seg = heap_kernel_alloc(ntohs(packet->length), 0);
                        memcpy(seg, packet, ntohs(packet->length));
                        con->rx = klist_add(con->rx, seg);
                        return 1;
                    }
                }
            }
            break;
        case TCP_SYN_RECV:
        {
            if (segment->ack && htonl(segment->ack_num) == htonl(sk->header.seq_num) + 1)
            {
                sk->state = TCP_ESTABLISHED;
                sk->header.ack_num = htonl(htonl(sk->header.ack_num) + 1);
                sk->header.seq_num = segment->ack_num;
                sk->header.ack = 0;
                sk->header.syn = 0;
            }
        }
        break;

        case TCP_ESTABLISHED:
            // sk->header.seq_num = sk->header.ack_num;
            sk->header.ack_num = htonl(__tcp_data_size(packet) + htonl(segment->seq_num));
            sk->header.ack = 1;
            tcp_send_segment(socket, NULL, NULL);
            
            uint32_t data_size = __tcp_data_size(packet);
            socket_entry_t *entry = heap_kernel_alloc(sizeof(socket_entry_t) + data_size, 0);
            entry->size = data_size;
            entry->timestamp = timestamp++;
            entry->addr.sa_data[0] = 47;
            memcpy((*entry).data, (void *)__tcp_data_ptr(segment), data_size);
            sk->rx = klist_add(sk->rx, (void *)entry);

            return 1;
        };
    }

    return 0;
}

uint16_t tcp_checksum(ipv4_packet_t *packet)
{
    tcp_segment_t *segment = (tcp_segment_t *)packet->data;

    uint32_t len = ntohs(packet->length) - (packet->ihl << 2);

    segment->checksum = 0;

    uint32_t sum = __ip_wrapsum(
        __ip_checksum(
            (unsigned char *)segment, sizeof(tcp_segment_t),
            __ip_checksum(segment->options_data, len - sizeof(tcp_segment_t),
                          __ip_checksum((unsigned char *)&(packet->src), 2 * sizeof(ipv4_addr_t),
                                        IP_PROTOCOL_TCP + len))));
    return segment->checksum = sum;
}

uint32_t tcp_send_segment(struct socket *socket, uint8_t *data_ptr, uint32_t data_size)
{
    tcp_socket_t *sk = socket->sk;

    nic_data_t *data = ipv4_create_packet(sk->device,
                                          IP_PROTOCOL_TCP,
                                          sk->remote.sin_addr.address,
                                          data_size + sizeof(tcp_segment_t) + 20);

    ipv4_packet_t *packet = data->frame + sizeof(ethernet_frame_t);
    tcp_segment_t *segment = &packet->data;
    memcpy(segment, &sk->header, sizeof(tcp_segment_t));
    segment->dst_port = sk->remote.sin_port;
    segment->src_port = sk->local.sin_port;
    // dont send any options
    segment->offset = 10;

    *(segment->options_data) = 0b00000010;
    *(segment->options_data + 1) = 0b00000100;
    *((uint16_t *)(segment->options_data + 2)) = htons(512);

    if (data_ptr && data_size)
        memcpy(segment->options_data + 20, data_ptr, data_size);

#ifndef TRUST_ME_BRO
    tcp_checksum(packet);
#endif
    return ipv4_send_packet(data);
}

static uint8_t *__tcp_data_ptr(tcp_segment_t *segment)
{
    return (uint8_t *)segment + (segment->offset << 2);
}

static uint32_t __tcp_data_size(ipv4_packet_t *packet)
{
    tcp_segment_t *segment = (tcp_segment_t *)(packet->data);
    return ntohs(packet->length) - (segment->offset << 2) - sizeof(tcp_segment_t);
}

static socket_t *__tcp_get_socket(struct sockaddr_in *addr)
{
    socket_t *ret = 0;
    for (uint32_t id = 1; id < SOCKET_DESCRIPTORS_COUNT; id++)
    {
        struct socket *socket = socket_descriptors[id];

        if (socket &&
            socket->domain == AF_INET &&
            socket->type == SOCK_STREAM &&
            socket->protocol == IP_PROTOCOL_TCP)
        {
            tcp_socket_t *sk = socket->sk;
            if (sk->local.sin_port == addr->sin_port)
            {
                if (sk->remote.sin_addr.address == addr->sin_addr.address)
                    // best match -- connection socket
                    return socket;
                else if (INADDR_BROADCAST == sk->local.sin_addr.address ||
                         0x7F == sk->local.sin_addr.address >> 24 ||
                         INADDR_ANY == sk->local.sin_addr.address)
                    // listen socket found
                    ret = socket;
            }
        }
    }

    return ret;
}

socket_t *tcp_socket_init(socket_t *socket)
{
    socket->state = SS_CONNECTED;
    socket->sk = heap_kernel_alloc(sizeof(tcp_socket_t), 0);
    tcp_socket_t *sk = socket->sk;
    sk->state = TCP_CLOSE;
    socket->ops = heap_kernel_alloc(sizeof(struct proto_ops), 0);
    memcpy(socket->ops, &tcp_interface, sizeof(struct proto_ops));

    return socket;
}

int tcp_socket_bind(struct socket *socket, struct sockaddr *addr, int sockaddr_len)
{
    tcp_socket_t *sk = socket->sk;
    struct sockaddr_in *_addr = (struct sockaddr_in *)addr;
    sk->local.sin_port = _addr->sin_port;
    sk->local.sin_addr.address = _addr->sin_addr.address;
    sk->device = network_manager_get_nic();
    sk->local.sin_family = AF_INET;
    return 0;
}

int tcp_socket_listen(struct socket *socket, int backlog)
{
    if (socket->state == SS_CONNECTED)
    {
        tcp_socket_t *sk = socket->sk;
        sk->state = TCP_LISTEN;
        // this socket is for listening for incoming connections,
        // prepate buffer for incoming connections:
        size_t con_list_size = sizeof(struct socket) * backlog;
        sk->connections = heap_kernel_alloc(con_list_size, 0);
        memset(sk->connections, 0, con_list_size);
        sk->backlog = backlog;

        for (uint32_t id = 0; id < backlog; id++)
        {
            struct socket *sock = &sk->connections[id];
            sock->state = SS_FREE;
            sock->sk = heap_kernel_alloc(sizeof(tcp_socket_t), 0);
            tcp_socket_t *sk = sock->sk;
            sk->state = TCP_CLOSE;
            sk->device = network_manager_get_nic();
            sk->local.sin_family = AF_INET;
            sk->local.sin_port = sk->local.sin_port;
            sk->local.sin_addr.address = sk->local.sin_addr.address;
            sk->rx = klist_init();
            sk->tx = klist_init();
            sock->ops = heap_kernel_alloc(sizeof(struct proto_ops), 0);
            memcpy(sock->ops, &tcp_interface, sizeof(struct proto_ops));
        }

        return 0;
    }
    else
        return -1;
}

int tcp_socket_accept(struct socket *socket, struct sockaddr *addr, int sockaddr_len)
{
    tcp_socket_t *sk = socket->sk;
    if (SS_CONNECTED == socket->state && TCP_LISTEN == sk->state)
    {
        // look for pending connections:
        for (int i = 0; i < sk->backlog; i++)
        {
            struct socket *sock = sk->connections + i;
            if (SS_CONNECTING == sock->state)
            {
                // try to establish new connection:
                volatile tcp_socket_t *con = sock->sk;
                ipv4_packet_t *pckt = con->rx->value;
                tcp_segment_t *syn = pckt->data;
                memcpy(&con->header, syn, sizeof(tcp_segment_t));

                memcpy(&con->local, &sk->local, sizeof(struct sockaddr_in));

                con->state = TCP_SYN_RECV;
                con->header.window = htons(512);
                con->header.ack_num = htonl(htonl(syn->seq_num) + 1);
                con->header.seq_num = htonl(4747);
                con->header.syn = 1;
                con->header.ack = 1;

                con->rx = klist_delete(con->rx, con->rx);
                int desc = socket_add_descriptor(sock);

                tcp_send_segment(sock, NULL, NULL);

                while (1)
                {
                    sleep(500);
                    if (TCP_ESTABLISHED == con->state)
                        break;
                    tcp_send_segment(sock, NULL, NULL);
                }

                return desc;
            }
        }
    }
    return -1;
}

int tcp_socket_recv(struct socket *sock, const void *buf, size_t len, int flags)
{
    tcp_socket_t *sk = (tcp_socket_t *)(sock->sk);

    if (((tcp_socket_t *)sock->sk)->state == TCP_ESTABLISHED && sk->rx && sk->rx->size)
    {
        socket_entry_t *entry = sk->rx->value;
        uint32_t size = entry->size;
        memcpy(buf, (*entry).data, size);
        sk->rx = klist_delete(sk->rx, sk->rx);
        return size;
    }
    else
        return 0;
}

int tcp_socket_send(struct socket *sock, void *buf, size_t len, int flags)
{
    return 0;
}

int tcp_socket_write(struct socket *socket, void *buf, size_t length, struct sockaddr *addr)
{
    // tcp_socket_conn_t *sk = (((tcp_socket_t *)socket->sk)->sock);
    // ipv4_packet_t *packet = (ipv4_packet_t *)buf;

    // void *seg = heap_kernel_alloc(ntohs(packet->length), 0);
    // memcpy(seg, packet, ntohs(packet->length));

    // sk->rx = klist_add(sk->rx, seg);
    logger_log_ok("Write to TCP Socket");
    // sk->rx->write = sk->buffer->write + 1 & (sk->buffer->length - 1);
    // socket_entry_t *entry = ((uint32_t) & (sk->buffer->entries) + ((size_t)(sk->buffer->write) * (sk->buffer->entry_size + (size_t)sizeof(socket_entry_t))));
    // if (addr)
    //     memcpy(&entry->addr, addr, sizeof(struct sockaddr_in));
    // memcpy(&entry->data, buf, length);
    // return entry->size = length;

    return 0;
}

int tcp_socket_read(struct socket *socket, void *buf, size_t length, struct sockaddr *addr)
{
    // tcp_socket_t *sk = socket->sk;
    // if (sk->buffer->entry_size > length)
    // {
    //     sk->buffer->write = sk->buffer->write + 1 & (sk->buffer->length - 1);
    //     socket_entry_t *entry = ((uint32_t) & (sk->buffer->entries) + ((size_t)(sk->buffer->write) * (sk->buffer->entry_size + (size_t)sizeof(socket_entry_t))));
    //     if (addr)
    //         memcpy(addr, &entry->addr, sizeof(struct sockaddr_in));
    //     memcpy(buf, &entry->data, length);
    //     return entry->size = length;
    // }
    // else
    //     return 0;
    return 0;
}

int tcp_socket_connect(struct socket *sock, struct sockaddr *vaddr, int sockaddr_len, int flags)
{
}
