#include "dhcp.h"

static uint32_t xid = 4323638;

static uint8_t parameter_request_list[] = {
    1,   // subnetmask
    28,  // broadcast addr
    2,   // time offset
    15,  // domain name
    6,   // domain name server
    119, // domain search
    12,  // hostname
    44,  // netbios tcp name server
    47,  // netbios tcp scpe
    26,  // interface mtu
    121, // classless static route
    42,  // ntp server
};

static uint8_t dhcp_offer[1500];

static int sock;

static struct sockaddr_in srv = {
    .sin_family = AF_INET,
    .sin_port = htons(BOOTP_SERVER),
    .sin_addr = {.oct_a = 0xFF,
                 .oct_b = 0xFF,
                 .oct_c = 0xFF,
                 .oct_d = 0xFF},
};

static struct sockaddr_in addr = {
    .sin_family = AF_INET,
    .sin_port = htons(BOOTP_CLIENT),
    .sin_addr.value = INADDR_ANY};

static void __dhcp_add_option_ptr(dhcp_message_t *msg, uint8_t option, uint8_t value[], uint8_t value_length)
{
    for (uint16_t i = 0; i < DHCP_OPTIONS_MAX; i++)
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
    for (uint16_t i = 0; i < DHCP_OPTIONS_MAX; i++)
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

uint32_t dhcp_negotiate(net_interface_t *interface)
{
    uint64_t elapsed;

    sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    bind(sock, &addr, sizeof(struct sockaddr_in));

    dhcp_discover(interface);
    printf("Sent DHCP discover\n");
    elapsed = 0;
    while (elapsed++ < 25)
    {
        micros_process_current_process_sleep(100);
        if (recvfrom(sock, &dhcp_offer, 1500, 0, &addr, sizeof(struct sockaddr_in)))
            break;
    }

    printf("Received DHCP offer\n");
    dhcp_process_offer(interface, &dhcp_offer);

    elapsed = 0;
    while (elapsed++ < 25)
    {
        micros_process_current_process_sleep(100);
        if (recvfrom(sock, &dhcp_offer, 1500, 0, &addr, sizeof(struct sockaddr_in)))
            break;
    }

    dhcp_process_offer(interface, &dhcp_offer);

    return 0;
}

void dhcp_discover(net_interface_t *interface)
{
    xid++;
    dhcp_message_t msg;
    memset(&msg, 0, sizeof(dhcp_message_t));

    msg.opcode = DHCP_DISCOVER;
    msg.htype = 0x1;
    msg.hlen = 0x6;
    msg.hops = 0;
    msg.broadcast = 1;
    msg.xid = htonl(xid);
    msg.magic_cookie = htonl(DHCP_MAGIC_COOKIE);

    msg.options[0] = DHCP_MESSAGE_TYPE;
    msg.options[1] = 0x01;
    msg.options[2] = DHCP_DISCOVER;
    msg.options[3] = 0xFF;

    memcpy(&msg.chaddr, interface->mac.addr, 6);
    sendto(sock, &msg, sizeof(dhcp_message_t), 0, &srv, sizeof(struct sockaddr_in));
}

int dhcp_process_offer(net_interface_t *interface, dhcp_message_t *dhcp_offer)
{
    uint8_t type;

    dhcp_read_option(dhcp_offer, DHCP_MESSAGE_TYPE, &type, 1);

    if (type == DHCP_OFFER)
    {
        dhcp_message_t dhcp_request;
        memset(&dhcp_request, 0, sizeof(dhcp_message_t));

        dhcp_request.opcode = DHCP_DISCOVER;
        dhcp_request.htype = 0x1;
        dhcp_request.hlen = 0x6;
        dhcp_request.hops = 0;
        dhcp_request.xid = htonl(xid);
        dhcp_request.ciaddr.value = 0;
        dhcp_request.siaddr.value = 0;
        dhcp_request.magic_cookie = htonl(DHCP_MAGIC_COOKIE);

        memcpy(&dhcp_request.options, (const uint8_t[]){[0 ... DHCP_OPTIONS_MAX - 1] = 0xFF}, DHCP_OPTIONS_MAX);

        dhcp_add_option(&dhcp_request, DHCP_MESSAGE_TYPE, (uint8_t)DHCP_REQUEST, 1);
        dhcp_add_option(&dhcp_request, DHCP_REQUESTED_IP, &dhcp_offer->yiaddr, sizeof(ipv4_addr_t));
        dhcp_add_option(&dhcp_request, 0xC, DHCP_HOSTNAME, strlen(DHCP_HOSTNAME));
        dhcp_add_option(&dhcp_request, 0x37, parameter_request_list, sizeof(parameter_request_list));

        memcpy(&dhcp_request.chaddr, &(*interface).mac, 6);
        sendto(sock, &dhcp_request, sizeof(dhcp_message_t), 0, &srv, sizeof(struct sockaddr_in));
    }
    else if (type == DHCP_ACK)
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

void dhcp_read_option(dhcp_message_t *msg, uint8_t code, uint8_t *buf, uint16_t len)
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
