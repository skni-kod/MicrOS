#include "dhcp.h"

void dhcp_discover(net_interface_t *interface);
int dhcp_handle_offer(net_interface_t *interface);
void dhcp_read_option(uint8_t *options,
                      uint8_t code,
                      uint8_t *buf,
                      uint16_t len);

static uint32_t xid = 4323638;

dhcp_message_t *dhcp_offer = NULL;
static char buffer[1500];
static int sock;

struct sockaddr_in srv = {
    .sin_family = AF_INET,
    .sin_port = htons(BOOTP_SERVER),
    .sin_addr = {.address = 0xFFFFFFFF},
};

struct sockaddr_in addr = {
    .sin_family = AF_INET,
    .sin_port = htons(BOOTP_CLIENT),
    .sin_addr.address = INADDR_ANY};

bool dhcp_negotiate(net_interface_t *interface)
{
    sock = k_socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    k_bind(sock, &addr, sizeof(struct sockaddr_in));

    dhcp_discover(interface);
    struct sockaddr_in addr;

    uint64_t elapsed = 0;
    while (elapsed++ < 0xFFFF)
    {
        if (k_recvfrom(sock, buffer, 1500, 0, &addr, sizeof(struct sockaddr_in)))
            break;
    }

    dhcp_offer = buffer;
    dhcp_handle_offer(interface);

    int recv;
    while (!(recv = k_recvfrom(sock, buffer, 1500, 0, &addr, sizeof(struct sockaddr_in))))
        ;

    dhcp_handle_offer(interface);
    return true;
}

void dhcp_discover(net_interface_t *interface)
{
    xid++;
    uint32_t msg_size = sizeof(dhcp_message_t) + 7;
    dhcp_message_t *msg = heap_kernel_alloc(msg_size, 0);

    msg->opcode = DHCP_DISCOVER;
    msg->htype = 0x1;
    msg->hlen = 0x6;
    msg->hops = 0;
    msg->broadcast_flag = 1;
    msg->flags1 = 0;
    msg->flags2 = 0;
    msg->xid = htonl(xid);
    msg->magic_cookie = htonl(DHCP_MAGIC_COOKIE);

    msg->options[0] = DHCP_MESSAGE_TYPE;
    msg->options[1] = 0x01;
    msg->options[2] = DHCP_DISCOVER;
    msg->options[3] = 0x37;
    msg->options[4] = 0x1;
    msg->options[5] = 0x1;
    msg->options[6] = 0xFF;

    memcpy(&msg->chaddr, interface->mac.addr, 6);

    k_sendto(sock, msg, msg_size, 0, &srv, sizeof(struct sockaddr_in));
}

void dhcp_receive_packet(net_interface_t *interface,
                         uint8_t *packet,
                         udp_datagram_t *header)
{

    dhcp_message_t dhcp_header = {0};
    memcpy(&dhcp_header, packet, sizeof(dhcp_message_t));
    dhcp_header.xid = ntohl(dhcp_header.xid);

    if (dhcp_header.xid != xid)
    {
        return;
    }

    switch (dhcp_header.opcode)
    {
    case DHCP_OFFER:
        dhcp_offer = heap_kernel_alloc(sizeof(dhcp_message_t), 0);
        memcpy(dhcp_offer, &dhcp_header, sizeof(dhcp_message_t));
        break;
    default:
        break;
    }
}

int dhcp_handle_offer(net_interface_t *interface)
{
    volatile uint8_t type[] = {0};
    dhcp_read_option(dhcp_offer->options, DHCP_MESSAGE_TYPE, type, 1);

    if (type[0] == DHCP_OFFER)
    {
        uint32_t req_size = sizeof(dhcp_message_t) + 16;
        dhcp_message_t *dhcp_request = heap_kernel_alloc(req_size, 0);

        dhcp_request->opcode = DHCP_DISCOVER;
        dhcp_request->htype = 0x1;
        dhcp_request->hlen = 0x6;
        dhcp_request->hops = 0;
        dhcp_request->xid = htonl(xid);
        dhcp_request->ciaddr = dhcp_offer->yiaddr;
        dhcp_request->siaddr = dhcp_offer->siaddr;
        dhcp_request->magic_cookie = htonl(DHCP_MAGIC_COOKIE);
        dhcp_request->options[0] = DHCP_MESSAGE_TYPE;
        dhcp_request->options[1] = 0x01;
        dhcp_request->options[2] = DHCP_REQUEST;
        dhcp_request->options[3] = DHCP_REQUESTED_IP;
        dhcp_request->options[4] = 0x04;
        dhcp_request->options[5] = ((ipv4_addr_t *)&dhcp_offer->yiaddr)->oct_a;
        dhcp_request->options[6] = ((ipv4_addr_t *)&dhcp_offer->yiaddr)->oct_b;
        dhcp_request->options[7] = ((ipv4_addr_t *)&dhcp_offer->yiaddr)->oct_c;
        dhcp_request->options[8] = ((ipv4_addr_t *)&dhcp_offer->yiaddr)->oct_d;
        dhcp_request->options[9] = DHCP_DHCP_SERVER;
        dhcp_request->options[10] = 0x04;
        dhcp_request->options[11] = ((ipv4_addr_t *)&dhcp_offer->siaddr)->oct_a;
        dhcp_request->options[12] = ((ipv4_addr_t *)&dhcp_offer->siaddr)->oct_b;
        dhcp_request->options[13] = ((ipv4_addr_t *)&dhcp_offer->siaddr)->oct_c;
        dhcp_request->options[14] = ((ipv4_addr_t *)&dhcp_offer->siaddr)->oct_d;
        dhcp_request->options[15] = 0xFF;

        memcpy(&dhcp_request->chaddr, &interface->mac, 6);

        k_sendto(sock, dhcp_request, req_size, 0, &srv, sizeof(struct sockaddr_in));
    }
    else if (type[0] == DHCP_ACK)
    {
        memcpy(&interface->ipv4_address.address, &dhcp_offer->yiaddr, 4);

        uint8_t gateway_ip[4] = {0};
        dhcp_read_option(dhcp_offer->options, DHCP_ROUTER, gateway_ip, 4);
        memcpy(&interface->ipv4_gateway.address, gateway_ip, 4);

        uint8_t dns_ip[4] = {0};
        dhcp_read_option(dhcp_offer->options, DHCP_DNS, dns_ip, 4);
        memcpy(&interface->ipv4_dns.address, dns_ip, 4);

        uint8_t netmask[4] = {0};
        dhcp_read_option(dhcp_offer->options, DHCP_NETMASK, netmask, 4);
        memcpy(&interface->ipv4_netmask.address, netmask, 4);
        return 1;
    }
    else
    {
    }
}

void dhcp_read_option(uint8_t *options,
                      uint8_t code,
                      uint8_t *buf,
                      uint16_t len)
{
    for (uint16_t i = 0; i < 1024; i++)
    {
        if (options[i] == code && options[i + 1] == len)
        {
            memcpy(buf, options + i + 2, len);
            break;
        }
    }
}
