#ifndef MICROS_SYSCALLS_H
#define MICROS_SYSCALLS_H

// Socket
#define SYSCALL_SOCKET          0x0E00
#define SYSCALL_BIND            0x0E01
#define SYSCALL_RECV            0x0E02
#define SYSCALL_RECVFROM        0x0E03
#define SYSCALL_SEND            0x0E04
#define SYSCALL_SENDTO          0x0E05
#define SYSCALL_LISTEN          0x0E06
#define SYSCALL_ACCEPT          0x0E07

#endif