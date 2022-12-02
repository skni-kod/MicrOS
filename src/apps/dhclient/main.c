#include "dhcp.h"

int main(int argc, char *argv[])
{
    if (argc < 3)
        return -1;
    uint32_t id = atoi(argv[2]);
    net_interface_t iface;
    micros_netif_get(id, &iface);
    dhcp_negotiate(&iface);
    micros_netif_set(id, &iface);
}