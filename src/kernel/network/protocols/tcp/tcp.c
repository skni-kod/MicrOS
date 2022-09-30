#include "tcp.h"

static struct proto_ops tcp_interface = {
    .bind = &tcp_socket_bind,
    .listen = &tcp_socket_listen,
    .accept = &tcp_socket_accept,
    .recv = &tcp_socket_recv,
    .send = &tcp_socket_send,
    .write = &tcp_socket_write,
    .read = &tcp_socket_read,
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
    socket_t *socket = socket_descriptor_lookup(AF_INET, SOCK_STREAM, IPPROTO_TCP, &addr);

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
            if (segment->syn)
            {
                struct socket *con = 0;
                for (int i = 0; i < sk->backlog; i++)
                {
                    struct socket *sock = (struct socket *)sk->rx + i;
                    sk = sock->sk;
                    if (sk->remote.sin_addr.address == packet->src.address &&
                        sk->remote.sin_port == segment->src_port)
                    {
                        con = sock;
                        tcp_socket_t *sk = sock->sk;
                        switch (sk->state)
                        {
                        case TCP_SYN_RECV:
                            if (segment->ack &&
                                htonl(segment->ack_num) - 1 == htonl(sk->header.ack_num))
                                sk->state = TCP_ESTABLISHED;
                            break;
                        case TCP_ESTABLISHED:
                            // write data to socket buffer
                            break;
                        };
                    }
                    else if (SS_FREE == sock->state)
                    {
                        con = sock;
                        // new client attempts to connect
                        sock->state = SS_CONNECTING;
                        sk->state = TCP_SYN_RECV;
                        sk->remote.sin_family = AF_INET;
                        sk->remote.sin_addr.address = packet->src.address;
                        sk->remote.sin_port = segment->src_port;
                        sk->rx = klist_init(sk->rx);
                        sk->tx = klist_init(sk->tx);
                        void *seg = heap_kernel_alloc(ntohs(packet->length), 0);
                        memcpy(seg, packet, ntohs(packet->length));
                        sk->rx = klist_add(sk->rx, seg);
                    }
                    else
                        continue;
                }
                if (!con)
                {
                    // refuse to accept new connection
                }
            }

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
                                          data_size + sizeof(tcp_segment_t));

    ipv4_packet_t *packet = data->frame + sizeof(ethernet_frame_t);
    tcp_segment_t *segment = &packet->data;
    memcpy(segment, &sk->header, sizeof(tcp_segment_t));
    segment->dst_port = sk->remote.sin_port;
    segment->src_port = sk->local.sin_port;

    if (data_ptr && data_size)
        memcpy(segment->options_data, data_ptr, data_size);

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

socket_t *tcp_socket_init(socket_t *socket)
{
    static uint16_t port = SOCKET_BASE_PORT;
    socket->state = SS_CONNECTED;
    socket->sk = heap_kernel_alloc(sizeof(tcp_socket_t), 0);
    tcp_socket_t *sk = socket->sk;
    sk->state = TCP_CLOSE;
    sk->device = network_manager_get_nic();
    sk->local.sin_family = AF_INET;
    sk->local.sin_port = SOCKET_BASE_PORT + port++;
    sk->local.sin_addr.address = sk->device->interface->ipv4.address;

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
    // TODO: check if port is open
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
        sk->rx = heap_kernel_alloc(con_list_size, 0);
        memset(sk->rx, 0, con_list_size);

        for (uint32_t id = 0; id < backlog; id++)
        {
            struct socket *sock = (struct socket *)sk->rx + id;
            sock->state = SS_FREE;
            sock->sk = heap_kernel_alloc(sizeof(tcp_socket_t), 0);
            ((tcp_socket_t *)sock->sk)->state = TCP_CLOSE;
            ((tcp_socket_t *)sock->sk)->device = network_manager_get_nic();
            ((tcp_socket_t *)sock->sk)->local.sin_family = AF_INET;
            ((tcp_socket_t *)sock->sk)->local.sin_port = sk->local.sin_port;
            ((tcp_socket_t *)sock->sk)->local.sin_addr.address = sk->local.sin_addr.address;
            sock->ops = heap_kernel_alloc(sizeof(struct proto_ops), 0);
            memcpy(sock->ops, &tcp_interface, sizeof(struct proto_ops));
        }

        sk->backlog = backlog;
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
            struct socket *sock = (struct socket *)sk->rx + i;
            tcp_socket_t *_sk = sock->sk;
            if (SS_CONNECTING == sock->state && TCP_SYN_RECV == _sk->state)
            {
                // try to establish new connection:
                ipv4_packet_t *pckt = _sk->rx->value;
                tcp_segment_t *syn = pckt->data;
                memcpy(&_sk->header, syn, sizeof(tcp_segment_t));
                _sk->header.window = syn->window;
                _sk->header.ack_num = htonl(htonl(syn->seq_num) + 1);
                _sk->header.seq_num = htonl(474747);
                _sk->header.syn = 1;
                _sk->header.ack = 1;

                tcp_send_segment(sock, __tcp_data_ptr(syn), __tcp_data_size(pckt));
                _sk->rx = klist_delete(_sk->rx, _sk->rx);

                // finally create new socket descriptor
                int con = socket_add_descriptor(sock);
                sk = sock->sk;
                sock->state = SS_CONNECTED;
                sk->state = TCP_ESTABLISHED;
                return con;
            }
        }

        // n_sk->state =

        // return new socket id
    }
    return -1;
}

int tcp_socket_recv(struct socket *sock, const void *buf, size_t len, int flags)
{
    // logger_log_info("Dupa, nie przeczytam");
    return 0;
}

int tcp_socket_send(struct socket *sock, void *buf, size_t len, int flags)
{
    // logger_log_info("Dupa, nie wysle");
    return 0;
}

int tcp_socket_write(struct socket *socket, void *buf, size_t len, struct sockaddr *addr)
{
}

int tcp_socket_read(struct socket *socket, void *buf, size_t length, struct sockaddr *addr)
{
}