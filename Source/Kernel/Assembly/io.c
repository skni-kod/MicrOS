#include "io.h"

/*static inline uint8_t inb(uint16_t port)
{
    uint8_t value;
    __asm__ volatile("inb %1, %0" : "=a"(value) : "Nd"(port));
    return value;
}

static inline void outb(uint16_t port, uint8_t val)
{
    __asm__ volatile("outb %0, %1" : : "a"(val), "Nd"(port));
}*/

void enable()
{
    __asm__ volatile("sti" ::);
}

void disable()
{
    __asm__ volatile("cli" ::);
}

void io_wait()
{
    __asm__ ( "outb %%al, $0x80" : : "a"(0) );
}