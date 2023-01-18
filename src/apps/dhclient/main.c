#include "dhcp.h"

static void __print_netif_info(net_interface_t *iface)
{
    printf("MAC: %02x:%02x:%02x:%02x:%02x:%02x\n",
           iface->mac.octet_a,
           iface->mac.octet_b,
           iface->mac.octet_c,
           iface->mac.octet_d,
           iface->mac.octet_e,
           iface->mac.octet_f);
    printf("IPv4: %d.%d.%d.%d\n",
           iface->ipv4_address.oct_a,
           iface->ipv4_address.oct_b,
           iface->ipv4_address.oct_c,
           iface->ipv4_address.oct_d);
    printf("Netmask: %d.%d.%d.%d\n",
           iface->ipv4_netmask.oct_a,
           iface->ipv4_netmask.oct_b,
           iface->ipv4_netmask.oct_c,
           iface->ipv4_netmask.oct_d);
    printf("GW: %d.%d.%d.%d\n",
           iface->ipv4_gateway.oct_a,
           iface->ipv4_gateway.oct_b,
           iface->ipv4_gateway.oct_c,
           iface->ipv4_gateway.oct_d);
    printf("DNS: %d.%d.%d.%d\n",
           iface->ipv4_dns.oct_a,
           iface->ipv4_dns.oct_b,
           iface->ipv4_dns.oct_c,
           iface->ipv4_dns.oct_d);
    printf("Dropped frames: %d\n", iface->frames_dropped);
    printf("RX frames: %d     bytes: %d\n", iface->frames_received, iface->bytes_received);
    printf("TX frames: %d     bytes: %d\n", iface->frames_sent, iface->bytes_sent);
}


int main(int argc, char *argv[])
{
    if (argc < 3)
        return -1;
    uint32_t id = atoi(argv[2]);
    net_interface_t iface;
    micros_netif_get(id, &iface);
    dhcp_negotiate(&iface);
    micros_netif_set(id, &iface);
    __print_netif_info(&iface);
}