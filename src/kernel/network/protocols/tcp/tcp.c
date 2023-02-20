#include "tcp.h"

static uint32_t timestamp = 0;
static uint16_t PORT = 1024;

static struct proto_ops tcp_interface = {
    .bind = &tcp_socket_bind,
    .connect = &tcp_socket_connect,
    .accept = &tcp_socket_accept,
    .listen = &tcp_socket_listen,
    .send = &tcp_socket_send,
    .sendto = &socket_not_implemented,
    .recv = &tcp_socket_recv,
    .recvfrom = &socket_not_implemented,
};

uint32_t tcp_process_segment(nic_data_t *data)
{
    ipv4_packet_t *packet = ((ethernet_frame_t *)(data->frame))->data;
    tcp_segment_t *segment = packet->data;

    char str[1024];
    kernel_sprintf(str,"%s",segment->options_data);
    logger_log_info(str);

#ifndef TRUST_ME_BRO
    // verify checksum
    uint16_t checksum = segment->checksum;
    tcp_checksum(packet);
    if (checksum != tcp_checksum(packet))
        return 0;
#endif

    struct sockaddr_in addr = {
        .sin_addr.value = packet->dst.value,
        .sin_port = segment->dst_port};

    // first look for open socket, so forward there incoming data:
    socket_t *socket = __tcp_get_socket(&addr);

    if (socket)
    {
        addr.sin_addr.value = packet->src.value;
        addr.sin_port = segment->src_port;
        tcp_socket_t *sk = socket->sk;

        // Here we have all of incoming segments, to our server eg. 192.168.1.165:80
        // we need to route segments, it we have active connections on that address,
        // or if address does not match, lets try to create new one
        switch (sk->state)
        {
        case TCP_LISTEN:
            // if incoming segment has syn flag
            if ((const tcp_flags_t){.syn = 1}.value == segment->flags.value)
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
                        con->remote.sin_addr.value = packet->src.value;
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
            if (segment->flags.ack && htonl(segment->ack_num) == htonl(sk->seq_num) + 1)
            {
                sk->state = TCP_ESTABLISHED;
                sk->seq_num = segment->ack_num;
            }
        }
        break;

        case TCP_ESTABLISHED:
            if ((const tcp_flags_t){.fin = 1, .ack = 1}.value == segment->flags.value)
            {
                sk->ack_num = htonl(TCP_DATA_SIZE(packet) + htonl(segment->seq_num) + 1);
                sk->seq_num = segment->ack_num;
                tcp_send_segment(socket, (tcp_flags_t){.ack = 1, .fin = 1}, NULL, NULL);
                sk->state = TCP_CLOSE;
                return 0;
            }

            if (segment->flags.ack)
            {
                sk->seq_num = segment->ack_num;
                sk->ack_num = htonl(TCP_DATA_SIZE(packet) + htonl(sk->ack_num));
            }

            if (segment->flags.psh & segment->flags.ack)
            {
                sk->rx = socket_add_entry(sk->rx, packet);
                tcp_send_segment(socket, (tcp_flags_t){.ack = 1}, NULL, NULL);
                tcp_flags_t *flags = &segment->flags;
            }
            return TCP_DATA_SIZE(packet);
        case TCP_LAST_ACK:
            if (segment->flags.fin && segment->flags.ack)
            {
                sk->ack_num = htonl(TCP_DATA_SIZE(packet) + htonl(segment->seq_num));
                sk->seq_num = segment->ack_num;
                tcp_send_segment(socket, (tcp_flags_t){.ack = 1}, NULL, NULL);
                sk->state = TCP_CLOSE;
                socket->state = SS_FREE;
                // TODO:
                // socket_remove(sk);
            }
            return 0;

        case TCP_SYN_SENT:
            if (segment->flags.ack && segment->flags.syn)
            {
                sk->state = TCP_ESTABLISHED;
                socket->state = SS_CONNECTED;
                sk->ack_num = htonl(TCP_DATA_SIZE(packet) + htonl(segment->seq_num) + 1);
                sk->seq_num = segment->ack_num;
                tcp_send_segment(socket, (tcp_flags_t){.ack = 1}, NULL, NULL);
            }
            break;
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

uint32_t tcp_send_segment(struct socket *socket, tcp_flags_t flags, uint8_t *data_ptr, uint32_t data_size)
{
    tcp_socket_t *sk = socket->sk;

    nic_data_t *data = ipv4_create_packet(sk->device,
                                          IP_PROTOCOL_TCP,
                                          sk->remote.sin_addr.value,
                                          data_size + sizeof(tcp_segment_t));

    ipv4_packet_t *packet = ((ethernet_frame_t *)(data->frame))->data;
    tcp_segment_t *segment = &packet->data;

    segment->ack_num = sk->ack_num;
    segment->seq_num = sk->seq_num;
    segment->pointer = sk->pointer;
    segment->window = sk->window;
    segment->flags = flags;
    segment->dst_port = sk->remote.sin_port;
    segment->src_port = sk->local.sin_port;
    segment->offset = 5;
    if (data_ptr && data_size)
        memcpy(segment->options_data, data_ptr, data_size);

#ifndef TRUST_ME_BRO
    tcp_checksum(packet);
#endif
    return ipv4_send_packet(data);
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

socket_t *tcp_socket_init(socket_t *socket)
{
    socket->state = SS_CONNECTED;
    socket->sk = heap_kernel_alloc(sizeof(tcp_socket_t), 0);
    tcp_socket_t *sk = socket->sk;
    sk->state = TCP_CLOSE;
    sk->local.sin_family = AF_INET;
    sk->local.sin_port = htons(PORT++);
    sk->local.sin_addr.value = INADDR_ANY;
    socket->ops = heap_kernel_alloc(sizeof(struct proto_ops), 0);
    memcpy(socket->ops, &tcp_interface, sizeof(struct proto_ops));

    return socket;
}

int tcp_socket_bind(struct socket *socket, struct sockaddr *addr, int sockaddr_len)
{
    tcp_socket_t *sk = socket->sk;
    struct sockaddr_in *_addr = (struct sockaddr_in *)addr;
    sk->local.sin_port = _addr->sin_port;
    sk->local.sin_addr.value = _addr->sin_addr.value;
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
            sk->local.sin_addr.value = sk->local.sin_addr.value;
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

                sk->remote.sin_addr.value = pckt->src.value;
                sk->remote.sin_port = syn->src_port;
                sk->remote.sin_family = AF_INET;

                memcpy(&con->local, &sk->local, sizeof(struct sockaddr_in));

                con->state = TCP_SYN_RECV;
                con->window = htons(512);
                con->ack_num = htonl(htonl(syn->seq_num) + 1);
                con->seq_num = htonl(4747);

                con->rx = klist_delete(con->rx, con->rx);
                int desc = socket_add_descriptor(sock);

                tcp_send_segment(sock, (tcp_flags_t){.ack = 1, .syn = 1}, NULL, NULL);

                for (int i = 0; i < 10; i++)
                {
                    sleep(500);
                    if (TCP_ESTABLISHED == con->state)
                        break;
                    tcp_send_segment(sock, (tcp_flags_t){.ack = 1, .syn = 1}, NULL, NULL);
                }

                memcpy(addr, &sk->remote, sizeof(struct sockaddr_in));

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
    tcp_socket_t *sk = (tcp_socket_t *)(sock->sk);

    if (((tcp_socket_t *)sock->sk)->state == TCP_ESTABLISHED)
    {
        tcp_send_segment(sock, (tcp_flags_t){.ack = 1, .psh = 1}, buf, len);
        return len;
    }
    else
        return 0;
}

int tcp_socket_connect(struct socket *socket, struct sockaddr *addr, int sockaddr_len, int flags)
{
    tcp_socket_t *sk = socket->sk;
    sk->device = network_manager_get_nic();
    sk->rx = klist_init();
    sk->tx = klist_init();

    memcpy(&sk->remote, addr, sizeof(struct sockaddr_in));

    socket->state = SS_CONNECTING;
    sk->state = TCP_SYN_SENT;
    sk->seq_num = htonl(234893);
    sk->ack_num = htonl(0);
    sk->pointer = 0;
    sk->window = htons(512);

    for (uint8_t attempts = 0; attempts < 10; attempts++)
    {
        tcp_send_segment(socket, (tcp_flags_t){.syn = 1}, NULL, NULL);
        uint32_t time = get_time();
        while (1)
        {
            if (get_time() - 1000 >= time)
                break;
            if (SS_CONNECTING != socket->state)
                break;
        }
        if (TCP_ESTABLISHED == sk->state)
            return 0;
    }

    return -1;
}
