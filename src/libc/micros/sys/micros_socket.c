#include "micros_socket.h"
#include "micros_process.h"

int socket(int domain, int type, int protocol)
{
#ifndef __libk
    return micros_interrupt_3a(SYSCALL_SOCKET, domain, type, protocol);
#else

#endif
}

int bind(int s, struct sockaddr *my_addr, socklen_t addrlen)
{
#ifndef __libk
    return micros_interrupt_3a(SYSCALL_BIND, s, my_addr, addrlen);
#else

#endif
}

int recv(int s, void *buf, size_t len, int flags)
{
#ifndef __libk
    struct recv_params params = {
        .s = s,
        .buf = buf,
        .len = len,
        .flags = flags,
        .from = NULL,
        .fromlen = NULL};

    return micros_interrupt_1a(SYSCALL_RECV, &params);
#else

#endif
}

int recvfrom(int s, void *buf, size_t len, int flags,
             struct sockaddr *from, socklen_t *fromlen)
{

#ifndef __libk
    struct recv_params params = {
        .s = s,
        .buf = buf,
        .len = len,
        .flags = flags,
        .from = from,
        .fromlen = fromlen};

    return micros_interrupt_1a(SYSCALL_RECVFROM, &params);
#else

#endif
}

int send(int s, const void *buf, size_t len, int flags)
{
#ifndef __libk
    struct sendto_params params = {
        .s = s,
        .buf = buf,
        .len = len,
        .flags = flags,
    };

    return micros_interrupt_1a(SYSCALL_SEND, &params);
#else

#endif
}

int sendto(int s, const void *buf, size_t len,
           int flags, const struct sockaddr *to,
           socklen_t tolen)
{
#ifndef __libk
    struct sendto_params params = {
        .s = s,
        .buf = buf,
        .len = len,
        .flags = flags,
        .to = to,
        .tolen = tolen};

    return micros_interrupt_1a(SYSCALL_SENDTO, &params);
#else

#endif
}

int listen(int s, int backlog)
{
#ifndef __libk
    return micros_interrupt_2a(SYSCALL_LISTEN, s, backlog);

#else

#endif
}

int accept(int s, struct sockaddr *addr, socklen_t *addrlen)
{
#ifndef __libk
    return micros_interrupt_3a(SYSCALL_ACCEPT, s, addr, addrlen);

#else

#endif
}

int connect(int s, struct sockaddr *serv_addr, int addrlen)
{
#ifndef __libk
    return micros_interrupt_3a(SYSCALL_CONNECT, s, serv_addr, addrlen);

#else

#endif
}
