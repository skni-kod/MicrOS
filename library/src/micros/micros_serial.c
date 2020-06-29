#include "micros_serial.h"

void micros_serial_init(unsigned int port, unsigned int baud_rate, unsigned int data_bits, unsigned int stop_bits, unsigned int parity)
{
    micros_interrupt_5a(0xD0, (uint32_t)port, (uint32_t)baud_rate, (uint32_t)data_bits, (uint32_t)stop_bits, (uint32_t)parity);
}

bool micros_serial_is_busy(unsigned int port)
{
    return micros_interrupt_1a(0xD1, (uint32_t)port);
}

bool micros_serial_is_queue_empty(unsigned int port)
{
    return micros_interrupt_1a(0xD2, (uint32_t)port);
}

void micros_serial_send(unsigned int port, char c)
{
    micros_interrupt_2a(0xD3, (uint32_t)port, (uint32_t)c);
}

void micros_serial_send_string(unsigned int port, char *str)
{
    micros_interrupt_2a(0xD4, (uint32_t)port, (uint32_t)str);
}

char micros_serial_receive(unsigned int port)
{
    return micros_interrupt_1a(0xD5, (uint32_t)port);
}