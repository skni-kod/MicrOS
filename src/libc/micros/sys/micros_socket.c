#include "micros_socket.h"
#include "micros_process.h"

int socket(int domain, int type, int protocol)
{
    return micros_interrupt_3a(SYSCALL_SOCKET, domain, type, protocol);
}

int bind(int s, struct sockaddr *my_addr, socklen_t addrlen)
{
    return micros_interrupt_3a(SYSCALL_BIND, s, my_addr, addrlen);
}

uint32_t recv(int s, void *buf, size_t len, int flags)
{
    return 0;
}

uint32_t recvfrom(int s, void *buf, size_t len, int flags,
                 struct sockaddr *from, socklen_t *fromlen)
{
    struct recv_params params = {
        .s = s,
        .buf = buf,
        .len = len,
        .flags = flags,
        .from = from,
        .fromlen = fromlen};

    return micros_interrupt_1a(SYSCALL_RECVFROM, &params);
}

uint32_t sendto(int s, const void *buf, size_t len,
               int flags, const struct sockaddr *to,
               socklen_t tolen)
{
    struct sendto_params params = {
        .s = s,
        .buf = buf,
        .len = len,
        .flags = flags,
        .to = to,
        .tolen = tolen
    };

    return micros_interrupt_1a(SYSCALL_SENDTO, &params);

}