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
    int idx = 0;
    static const char lowercase_table[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};

    bool negative = input < 0;
    if (negative)
        input *= -1;

    // put number to buffer in reverse order
    do
    {
        size_t digit = (input % base);
        buffer[idx++] = lowercase_table[digit];

        input /= base;
    } while (input);

    //put sign
    if (negative)
    {
        buffer[idx++] = '-';
    }

    int size = idx;
    idx--;

    // revert string
    for (size_t i = 0; i < size / 2; i++)
    {
        char tmp = buffer[i];
        buffer[i] = buffer[idx];
        buffer[idx] = tmp;

        idx--;
    }

    buffer[size] = '\0';

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
        seed = time(NULL);
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