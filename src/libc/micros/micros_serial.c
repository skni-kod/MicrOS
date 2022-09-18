#include "micros_serial.h"

void micros_serial_init(unsigned int port, unsigned int baud_rate, unsigned int data_bits, unsigned int stop_bits, unsigned int parity)
{
#ifndef __libk 

    micros_interrupt_5a(0x0D00, (uint32_t)port, (uint32_t)baud_rate, (uint32_t)data_bits, (uint32_t)stop_bits, (uint32_t)parity);

#else

#endif
}

bool micros_serial_is_busy(unsigned int port)
{
#ifndef __libk 

    return micros_interrupt_1a(0x0D01, (uint32_t)port);

#else

#endif
}

bool micros_serial_is_queue_empty(unsigned int port)
{
#ifndef __libk 

    return micros_interrupt_1a(0x0D02, (uint32_t)port);

#else

#endif
}

void micros_serial_send(unsigned int port, char c)
{
#ifndef __libk 

    micros_interrupt_2a(0x0D03, (uint32_t)port, (uint32_t)c);

#else

#endif
}

void micros_serial_send_string(unsigned int port, char *str)
{
#ifndef __libk 

    micros_interrupt_2a(0x0D04, (uint32_t)port, (uint32_t)str);

#else

#endif
}

char micros_serial_receive(unsigned int port)
{
#ifndef __libk 

    return micros_interrupt_1a(0x0D05, (uint32_t)port);

#else

#endif
}