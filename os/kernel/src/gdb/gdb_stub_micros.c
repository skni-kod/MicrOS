#include "cpu/idt/idt.h"
#include "drivers/serial/serial.h"

int getDebugChar()
{
    return serial_receive(COM1_PORT);
}

void putDebugChar(int c)
{
    serial_send(COM1_PORT, c);
}

void exceptionHandler(int exception_number, void *exception_address)
{
    idt_attach_exception_handler(exception_number, exception_address);
}