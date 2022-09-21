#ifndef KQUEUE_H
#define KQUEUE_H

#include <stdint.h>
#include <string.h>
#include <memory/heap/heap.h>

typedef struct kqueue{
    uint32_t read;
    uint32_t write;
    uint32_t size;
    size_t block_size;
    uint8_t data[];
} kqueue_t;

kqueue_t *kqueue_init(size_t blocks, size_t block_size);

size_t read(kqueue_t *queue, void *buffer);

size_t write(kqueue_t *queue, void *buffer);


#endif