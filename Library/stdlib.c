#include "stdlib.h"
#include "string.h"

unsigned int seed = 0;

int32_t atoi(const char *string)
{
    size_t len = strlen(string);
    int32_t value = 0;
    int32_t position = 1;

    char sign = 1;
    char lengthOffset = 0;
    if (*string == '-')
    {
        sign = -1;
        lengthOffset = 1;
    }

    for (int8_t i = len - 1; i >= lengthOffset; --i)
    {
        value += (string[i] - '0') * position;
        position *= 10;
    }

    return value * sign;
}

char *itoa(int input, char *buffer, int base)
{
    int8_t index = 0;
    int8_t kappa = 0;

    int8_t sign = 0;
    if (input < 0)
    {
        input *= -1;
        sign = 1;
    }

    do
    {
        kappa = input % base;
        buffer[index] = (kappa >= 10 ? kappa - 10 + 'a' : kappa + '0');

        input /= base;

        ++index;
    } while (input > 0);

    if (sign)
    {
        buffer[index] = '-';
        ++index;
    }

    for (int i = 0; i < index / 2; i++)
    {
        char tmp = buffer[i];
        buffer[i] = buffer[index - i - 1];
        buffer[index - i - 1] = tmp;
    }

    buffer[index] = '\0';

    return buffer;
}

void *malloc(size_t size)
{
    return micros_heap_alloc(size, 0);
}

void *malloc_align(size_t size, uint32_t align)
{
    return micros_heap_alloc(size, align);
}

void *calloc(size_t num, size_t size)
{
    size_t total_size = num * size;

    void *ptr = micros_heap_alloc(total_size, 0);
    memset(ptr, 0, total_size);

    return ptr;
}

void *realloc(void *ptr, size_t size)
{
    return micros_heap_realloc(ptr, size);
}

void free(void *ptr)
{
    micros_heap_dealloc(ptr);
}

int rand()
{
    const int multiplier = 1103515245;
    const int increment = 12345;
    const int modulus = 2147483648;

    if (seed == 0)
    {
        // TODO: Use time() instead of clock()
        seed = clock();
    }

    seed = (multiplier * seed + increment) % modulus;
    return seed;
}

void srand(unsigned int new_seed)
{
    seed = new_seed;
}

void abort()
{
    micros_process_exit(-1);
}

void exit(int status)
{
    micros_process_exit(status);
}