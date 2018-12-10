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

uint16_t inw(uint16_t port)
{
    uint16_t value;
    
    __asm__ ("inw %1, %0" : "=a"(value) : "Nd"(port));
    io_wait();

    return value;
}
void outw(uint16_t port, uint16_t val)
{
    __asm__ ("outw %0, %1" : : "a"(val), "Nd"(port));
    io_wait();
} 
uint32_t inl(uint16_t port)
{
    uint32_t value;
    
    __asm__ ("inl %1, %0" : "=a"(value) : "Nd"(port));
    io_wait();

    return value;
}
void outl(uint16_t port, uint32_t val){
    __asm__ ("outl %0, %1" : : "a"(val), "Nd"(port));
    io_wait();
}

void enable()
{
    __asm__ ("sti" ::);
}

void disable()
{
    __asm__ ("cli" ::);
}

void io_wait()
{
    __asm__ ( "outb %%al, $0x80" : : "a"(0) );
}