#include <stdint.h>
#include <time.h>
#include <stdlib.h>
#include <time.h>

int test_array[1024];

int main(int id)
{
    char buffer[32];

    call_interrupt_1a(0x04, "Process START ");
    itoa(id, buffer, 10);
    call_interrupt_1a(0x04, buffer);
    call_interrupt_1a(0x04, "\n");

    uint32_t asd = 100;
    for (int i = 0; i < id; i++)
    {
        asd *= 10;
    }

    while (1)
    {
        asd++;

        call_interrupt_1a(0x04, "Process ");
        itoa(id, buffer, 10);
        call_interrupt_1a(0x04, buffer);
        call_interrupt_1a(0x04, ": ");
        itoa(asd, buffer, 10);
        call_interrupt_1a(0x04, buffer);
        call_interrupt_1a(0x04, "\n");

        sleep(30);
    }
}