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

int recv(int s, void *buf, size_t len, int flags)
{
    struct recv_params params = {
        .s = s,
        .buf = buf,
        .len = len,
        .flags = flags,
        .from = NULL,
        .fromlen = NULL};

    return micros_interrupt_1a(SYSCALL_RECV, &params);
}

int recvfrom(int s, void *buf, size_t len, int flags,
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

int send(int s, const void *buf, size_t len,
         int flags, const struct sockaddr *to,
         socklen_t tolen)
{
    struct sendto_params params = {
        .s = s,
        .buf = buf,
        .len = len,
        .flags = flags,
    };

    return micros_interrupt_1a(SYSCALL_SEND, &params);
}

int sendto(int s, const void *buf, size_t len,
           int flags, const struct sockaddr *to,
           socklen_t tolen)
{
    struct sendto_params params = {
        .s = s,
        .buf = buf,
        .len = len,
        .flags = flags,
        .to = to,
        .tolen = tolen};

    return micros_interrupt_1a(SYSCALL_SENDTO, &params);
}

int listen(int s, int backlog)
{
    return micros_interrupt_2a(SYSCALL_LISTEN, s, backlog);
}

int accept(int s, struct sockaddr *addr, socklen_t *addrlen)
{
    return micros_interrupt_3a(SYSCALL_ACCEPT, s, addr, addrlen);
}

int connect(int s, struct sockaddr *serv_addr, int addrlen)
{
    return micros_interrupt_3a(SYSCALL_CONNECT, s, serv_addr, addrlen);
}
