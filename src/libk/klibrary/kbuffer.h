#ifndef KBUFFER_H
#define KBUFFER_H

#include <stdint.h>
#include <stdbool.h>
#include <memory/heap/heap.h>

// circular buffer

typedef struct kentry
{
  uint32_t size;
  uint8_t used;
  void *data[];
} kentry_t;

typedef struct kbuffer
{
  uint32_t block_count;
  uint32_t block_size;
  uint32_t ptr;
  void *entries[];
} kbuffer_t;

// create memory area with fixed size, and divide it into fixed size blocks,
// when kbuffer_get is called it allocates continous part of memory area
kbuffer_t *kbuffer_init(uint32_t block_size, uint32_t block_count);

void *kbuffer_get(kbuffer_t *buffer, uint32_t size);

// as parameter pass value got by kbuffer_get
void kbuffer_drop(void *ptr);

#endif