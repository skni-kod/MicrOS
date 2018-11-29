#ifndef VECTOR_H
#define VECTOR_H

#include "stdlib.h"

typedef struct vector
{
	void** data;
	int size;
	int count;
} vector;

void vector_init(vector* vector);
void vector_add(vector* vector, void* data);
void vector_insert(vector* vector, void* data, int index);
void vector_remove(vector* vector, int index);
void vector_clear(vector* vector);

#endif