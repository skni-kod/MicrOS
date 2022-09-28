#include "network_socket.h"

socket_t *network_init_socket(socket_t *socket)
{
    switch (socket->protocol)
    {
    case IP_PROTOCOL_UDP:
        return udp_socket_init(socket);
    case IP_PROTOCOL_TCP:
        return tcp_socket_init(socket);
    };
}
