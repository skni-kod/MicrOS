#include "io.h"

uint8_t inb(uint16_t port)
{
    uint8_t value;
    __asm__ ("inb %1, %0" : "=a"(value) : "Nd"(port));
    return value;
}

void outb(uint16_t port, uint8_t val)
{
    __asm__ ("outb %0, %1" : : "a"(val), "Nd"(port));
}

void enable()
{
    __asm__ ("sti" ::);
}