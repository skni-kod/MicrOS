#include "tcp.h"

static struct proto_ops tcp_interface = {
    // .bind = &tcp_socket_bind,
    // .listen = &tcp_socket_bind,
    // .recv = &tcp_socket_recvfrom,
    // .send = &tcp_socket_sendto,
    // .write = &tcp_socket_write,
    // .read = &tcp_socket_read,
};

void tcp_process_segment(nic_data_t *data)
{
    ipv4_packet_t *packet = (ipv4_packet_t *)(data->frame + sizeof(ethernet_frame_t));
    tcp_segment_t *segment = packet->data;

#ifndef TRUST_ME_BRO
    // verify checksum
    uint16_t checksum = segment->checksum;
    tcp_checksum(packet);
    if(checksum != tcp_checksum(packet))
        return;
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
    }
    else
    {
    }
}

uint16_t tcp_checksum(ipv4_packet_t *packet)
{
    /* Check the IP header checksum - it should be zero. */
    if (__ip_wrapsum(__ip_checksum((unsigned char *)packet, sizeof(ipv4_packet_t), 0)) != 0)
        return -1;

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

nic_data_t *tcp_create_segment(net_device_t *device, ipv4_addr_t dst_addr, uint16_t dst_port, uint16_t src_port, uint32_t data_size)
{
    // nic_data_t *data = ipv4_create_packet(device, IP_PROTOCOL_UDP, dst_addr, data_size + sizeof(tcp_segment_t));
    // ipv4_packet_t *packet = data->frame + sizeof(ethernet_frame_t);
    // udp_datagram_t *datagram = &packet->data;
    // datagram->dst_port = dst_port;
    // datagram->src_port = src_port;
    // datagram->length = htons((data_size + sizeof(udp_datagram_t)));
    // return data;
}

uint32_t tcp_send_segment(nic_data_t *data)
{
    ipv4_packet_t *packet = data->frame + sizeof(ethernet_frame_t);
    tcp_checksum(packet);

    return ipv4_send_packet(data);
}

static uint8_t *__tcp_data_ptr(tcp_segment_t *segment)
{
    return (uint8_t *)segment + (segment->offset << 2);
}

static uint32_t __tcp_data_size(ipv4_packet_t *packet)
{
    tcp_segment_t *segment = (tcp_segment_t *)(packet->data);
    return ntohs(packet->length) - (segment->offset << 2);
}

socket_t *tcp_socket_init(socket_t *socket)
{
    static uint16_t port = 2022;
    socket->state = SS_CONNECTING;
    socket->sk = heap_kernel_alloc(sizeof(udp_socket_t), 0);
    udp_socket_t *sk = socket->sk;
    sk->buffer = socket_init_buffer(sk->buffer, SOCKET_BUFFER_ENTRY_SIZE, SOCKET_BUFFER_ENTRY_COUNT);
    sk->device = network_manager_get_nic();
    sk->local.sin_family = AF_INET;
    sk->local.sin_port = SOCKET_BASE_PORT + port++;
    sk->local.sin_addr.address = sk->device->interface->ipv4.address;

    socket->ops = heap_kernel_alloc(sizeof(struct proto_ops), 0);
    memcpy(socket->ops, &tcp_interface, sizeof(struct proto_ops));

    return socket;
}

