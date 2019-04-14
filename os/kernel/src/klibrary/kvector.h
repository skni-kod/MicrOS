#ifndef KVECTOR_H
#define KVECTOR_H

#include <string.h>
#include "memory/heap/heap.h"

typedef struct kvector
{
	void **data;
	uint32_t size;
	uint32_t count;
} kvector;

void kvector_init(kvector *vector);
void kvector_add(kvector *vector, void *data);
void kvector_insert(kvector *vector, void *data, uint32_t index);
void kvector_remove(kvector *vector, uint32_t index);
void kvector_clear(kvector *vector);

#endif