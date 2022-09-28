#ifndef network_socket
#define network_socket

#include <process/socket/socket.h>
#include "protocols/udp/udp.h"
#include "protocols/tcp/tcp.h"

socket_t *network_init_socket(socket_t *socket);

#endif