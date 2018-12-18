#include <stdint.h>
#include <time.h>
#include <stdlib.h>

int test_array[1024];

int main()
{
    uint32_t start_time;
    start_time = clock();

    while (1)
    {
        int asd = clock();

        char buffer[32];
        itoa(asd, buffer, 10);
        call_interrupt_1a(0x04, "Process 1: ");
        call_interrupt_1a(0x04, buffer);
        call_interrupt_1a(0x04, "\n");
    }
}