#include "io.h"

inline uint8_t io_in_byte(uint16_t port)
{
    uint8_t value;

    __asm__("inb %1, %0"
            : "=a"(value)
            : "Nd"(port));
    io_wait();

    return value;
}

inline void io_out_byte(uint16_t port, uint8_t val)
{
    __asm__("outb %0, %1"
            :
            : "a"(val), "Nd"(port));
    io_wait();
}

inline uint16_t io_in_word(uint16_t port)
{
    uint16_t value;

    __asm__("inw %1, %0"
            : "=a"(value)
            : "Nd"(port));
    io_wait();

    return value;
}
inline void io_out_word(uint16_t port, uint16_t val)
{
    __asm__("outw %0, %1"
            :
            : "a"(val), "Nd"(port));
    io_wait();
}
inline uint32_t io_in_long(uint16_t port)
{
    uint32_t value;

    __asm__("inl %1, %0"
            : "=a"(value)
            : "Nd"(port));
    io_wait();

    return value;
}
inline void io_out_long(uint16_t port, uint32_t val)
{
    __asm__("outl %0, %1"
            :
            : "a"(val), "Nd"(port));
    io_wait();
}

inline void io_enable_interrupts()
{
    __asm__("sti" ::);
}

inline void io_disable_interrupts()
{
    __asm__("cli" ::);
}

inline void io_wait()
{
    __asm__("outb %%al, $0x80"
            :
            : "a"(0));
}