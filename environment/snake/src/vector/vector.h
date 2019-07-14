#ifndef VECTOR_H
#define VECTOR_H

#include <string.h>
#include <stdlib.h>

typedef struct vector
{
	void **data;
	uint32_t size;
	uint32_t count;
} vector;

void vector_init(vector *vector);
void vector_add(vector *vector, void *data);
void vector_insert(vector *vector, void *data, uint32_t index);
void vector_remove(vector *vector, uint32_t index);
void vector_clear(vector *vector);

#endif