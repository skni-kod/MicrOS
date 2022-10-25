#ifndef MICROS_SYSCALLS_H
#define MICROS_SYSCALLS_H

// Socket
#define SYSCALL_SOCKET                          0x0E00
#define SYSCALL_BIND                            0x0E01
#define SYSCALL_RECV                            0x0E02
#define SYSCALL_RECVFROM                        0x0E03
#define SYSCALL_SEND                            0x0E04
#define SYSCALL_SENDTO                          0x0E05
#define SYSCALL_LISTEN                          0x0E06
#define SYSCALL_ACCEPT                          0x0E07
#define SYSCALL_CONNECT                         0x0E08

// Network Interface
#define SYSCALL_NETIF_DROPPED                   0x0E35
#define SYSCALL_NETIF_GET_IPv4_ADDRESS          0x0E50
#define SYSCALL_NETIF_GET_IPv4_NETMASK          0x0E51
#define SYSCALL_NETIF_GET_IPv4_GW               0x0E52
#define SYSCALL_NETIF_GET_IPv4_DNS              0x0E53


// Other, eg. DNS, or DHCP 
#define SYSCALL_DNS_LOOKUP                      0x0E80

#endif