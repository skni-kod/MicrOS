#include "io.h"

uint8_t inb(uint16_t port)
{
    uint8_t value;
    
    __asm__ ("inb %1, %0" : "=a"(value) : "Nd"(port));
    io_wait();

    return value;
}

void outb(uint16_t port, uint8_t val)
{
    __asm__ ("outb %0, %1" : : "a"(val), "Nd"(port));
    io_wait();
}

void enable()
{
    __asm__ ("sti" ::);
}

void io_wait()
{
    __asm__ ( "outb %%al, $0x80" : : "a"(0) );
}