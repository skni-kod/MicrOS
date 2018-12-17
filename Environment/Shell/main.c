#include <stdint.h>
#include <time.h>

int test_array[1024];

int main()
{
    uint32_t start_time = clock();
    while (1)
    {
        uint32_t test_clock = clock();
        test_clock++;
    }
}