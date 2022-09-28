#include "micros_netif.h"
#include "micros_process.h"

uint32_t nic_dropped()
{
    return micros_interrupt_0a(SYSCALL_NETIF_DROPPED);
}
