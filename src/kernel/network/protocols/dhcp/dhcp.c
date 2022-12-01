#include "dhcp.h"

void dhcp_discover(net_interface_t *interface);
int dhcp_handle_offer(net_interface_t *interface);
void dhcp_read_option(dhcp_message_t *msg,
                      uint8_t code,
                      uint8_t *buf,
                      uint16_t len);

static uint32_t xid = 4323638;

static uint8_t param_request_list[] = {
    0x1, // subnetmask
    28,  // bcast addr
    2,   // time offset
    15,  // domain name
    6,   // domain name server
    119, // domain search
    12,  // hostname
    44,  // netbio tcp name server
    47,  // netbios tcp scpe
    26,  // interface mtu
    121, // classless static route
    42,  // ntp server
};

dhcp_message_t *dhcp_offer = NULL;
static char buffer[1500];
static int sock;

struct sockaddr_in srv = {
    .sin_family = AF_INET,
    .sin_port = htons(BOOTP_SERVER),
    .sin_addr = {.oct_a = 0xFF,
                 .oct_b = 0xFF,
                 .oct_c = 0xFF,
                 .oct_d = 0xFF},
};

struct sockaddr_in addr = {
    .sin_family = AF_INET,
    .sin_port = htons(BOOTP_CLIENT),
    .sin_addr.value = INADDR_ANY};

static void __dhcp_add_option_ptr(dhcp_message_t *msg, uint8_t option, uint8_t value[], uint8_t value_length)
{
    // look for no options end
    for (uint16_t i = 0; i < 68; i++)
    {
        uint8_t tmp = msg->options[i];
        if (tmp == 0xFF)
        {
            msg->options[i] = option;
            msg->options[i + 1] = value_length;
            memcpy(msg->options + i + 2, value, value_length);
            msg->options[i + 2 + value_length] = 0xFF;
            break;
        }
    }
}

static void __dhcp_add_option(dhcp_message_t *msg, uint8_t option, uint8_t value)
{
    // look for no options end
    for (uint16_t i = 0; i < 68; i++)
    {
        uint8_t tmp = msg->options[i];
        if (tmp == 0xFF)
        {
            msg->options[i] = option;
            msg->options[i + 1] = 1;
            msg->options[i + 2] = value;
            msg->options[i + 3] = 0xFF;
            break;
        }
    }
}

static void __dhcp_print_message(dhcp_message_t *msg)
{
    char string[1024];
    uint8_t type;
    dhcp_read_option(msg, DHCP_MESSAGE_TYPE, &type, 1);

    kernel_sprintf(string,
                   "OPCODE: %d\nhtype: %d\nhlen: %d\ndhcp_type: %d\n",
                   msg->opcode, msg->htype, msg->hlen, type);
    logger_replace_info(string);

    uint32_t i = 0;
    while (1)
    {
        uint8_t type = msg->options[i];
        uint8_t len = msg->options[i + 1];
        uint8_t *value = ((*msg).options + i + 2);

        if (0xFF == type)
            return;

        sprintf(string, "OPTION: %x, LEN: %x\n%.*s", type, len, len, value);

        i += *((*msg).options + 1);
    }
}

uint32_t dhcp_negotiate(net_interface_t *interface)
{
    sock = k_socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    k_bind(sock, &addr, sizeof(struct sockaddr_in));

    dhcp_discover(interface);

    struct sockaddr_in addr;
    int recv;

    uint64_t elapsed = 0;
    while (elapsed++ < 0xFFFFF)
    {
        if (recv = k_recvfrom(sock, buffer, 1500, 0, &addr, sizeof(struct sockaddr_in)))
            break;
    }

    dhcp_message_t *msg = buffer;

    dhcp_offer = buffer;
    dhcp_handle_offer(interface);

    elapsed = 0;
    while (elapsed++ < 0xFFFFF)
    {
        if (recv = k_recvfrom(sock, buffer, 1500, 0, &addr, sizeof(struct sockaddr_in)))
            break;
    }

    dhcp_handle_offer(interface);

    return 0;
}

void dhcp_discover(net_interface_t *interface)
{
    xid++;
    uint32_t msg_size = sizeof(dhcp_message_t);
    dhcp_message_t *msg = heap_kernel_alloc(msg_size, 0);
    memset(msg, 0, msg_size);

    msg->opcode = DHCP_DISCOVER;
    msg->htype = 0x1;
    msg->hlen = 0x6;
    msg->hops = 0;
    msg->broadcast = 1;
    msg->xid = htonl(xid);
    msg->magic_cookie = htonl(DHCP_MAGIC_COOKIE);

    msg->options[0] = DHCP_MESSAGE_TYPE;
    msg->options[1] = 0x01;
    msg->options[2] = DHCP_DISCOVER;
    msg->options[3] = 0xFF;

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
    uint8_t type[] = {0};
    dhcp_read_option(dhcp_offer, DHCP_MESSAGE_TYPE, type, 1);

    if (type[0] == DHCP_OFFER)
    {
        uint32_t req_size = sizeof(dhcp_message_t);
        dhcp_message_t *dhcp_request = heap_kernel_alloc(req_size, 0);
        memset(dhcp_request, 0, req_size);

        dhcp_request->opcode = DHCP_DISCOVER;
        dhcp_request->htype = 0x1;
        dhcp_request->hlen = 0x6;
        dhcp_request->hops = 0;
        dhcp_request->xid = htonl(xid);
        dhcp_request->ciaddr.value = 0;
        dhcp_request->siaddr.value = 0;
        dhcp_request->magic_cookie = htonl(DHCP_MAGIC_COOKIE);
        dhcp_request->options[0] = 0xFF;
        __dhcp_add_option(dhcp_request, DHCP_MESSAGE_TYPE, DHCP_REQUEST);
        __dhcp_add_option_ptr(dhcp_request, DHCP_REQUESTED_IP, &dhcp_offer->yiaddr, sizeof(ipv4_addr_t));
        __dhcp_add_option_ptr(dhcp_request, 0xC, "MicrOS", strlen("MicrOS"));
        __dhcp_add_option_ptr(dhcp_request, 0x37, param_request_list, sizeof(param_request_list));

        memcpy(&(*dhcp_request).chaddr, &(*interface).mac, 6);

        k_sendto(sock, dhcp_request, req_size, 0, &srv, sizeof(struct sockaddr_in));
    }
    else if (type[0] == DHCP_ACK)
    {
        memcpy(&interface->ipv4_address.value, &dhcp_offer->yiaddr.value, 4);

        dhcp_read_option(dhcp_offer, DHCP_ROUTER, &interface->ipv4_gateway.value, 4);

        dhcp_read_option(dhcp_offer, DHCP_DNS, &interface->ipv4_dns.value, 4);

        dhcp_read_option(dhcp_offer, DHCP_NETMASK, &interface->ipv4_netmask.value, 4);

        dhcp_read_option(dhcp_offer, DHCP_LEASE_TIME, &interface->ipv4_lease_time, 4);

        interface->ipv4_lease_time = ntohl(interface->ipv4_lease_time);
        return 0;
    }

    return -1;
}

void dhcp_read_option(dhcp_message_t *msg,
                      uint8_t code,
                      uint8_t *buf,
                      uint16_t len)
{
    uint32_t i = 0;
    while (1)
    {
        uint8_t type = msg->options[i];

        if (0xFF == type)
            return;

        if (type == code)
        {
            memcpy(buf, (*msg).options + i + 2, len);
            return;
        }
        int skip = msg->options[i + 1] + 2;
        i += skip;
    }
}
