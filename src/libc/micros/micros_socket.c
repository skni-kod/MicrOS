#include "micros_socket.h"

uint32_t micros_socket(uint32_t domain, uint32_t type, uint32_t protocol, uint32_t port)
{
    micros_interrupt_4a(0x0E00, domain, type, protocol,port);
}

uint32_t micros_recv(uint32_t sock, void* buffer, size_t len)
{
    uint32_t value = 0;
    while(!(value = micros_interrupt_3a(0x0E01, sock, buffer, len)));
    return value;
}