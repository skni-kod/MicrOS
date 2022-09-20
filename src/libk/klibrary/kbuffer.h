#ifndef KBUFFER_H
#define KBUFFER_H

#include <stdint.h>
#include <stdbool.h>
#include <memory/heap/heap.h>

// circular buffer

typedef struct kentry{
    uint8_t used;
    uint8_t data[];
} kentry_t;

typedef struct kbuffer
{
  uint32_t length;
  uint32_t data_size;
  uint32_t ptr;
  uint8_t entries[];
} kbuffer_t;

int kbuffer_init(kbuffer_t *buffer, uint16_t entry_size, uint16_t entry_count);

void* kbuffer_get(kbuffer_t *buffer);

// as parameter pass value got by kbuffer_get
void* kbuffer_drop(void* buffer);

#endif