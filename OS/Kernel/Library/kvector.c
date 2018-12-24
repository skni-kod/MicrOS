#include "kvector.h"

void kvector_init(kvector *vector)
{
	vector->data = 0;
	vector->size = 0;
	vector->count = 0;
}

void kvector_add(kvector *vector, void *data)
{
	kvector_insert(vector, data, vector->count);
}

void kvector_insert(kvector *vector, void *data, uint32_t index)
{
	if (vector->size == 0)
	{
		vector->size = 8;
		vector->data = heap_kernel_alloc(sizeof(void *) * vector->size, 0);
	}

	if (vector->count + 1 > vector->size)
	{
		vector->size *= 2;
		vector->data = heap_kernel_realloc(vector->data, sizeof(void *) * vector->size, 0);
	}

	for (uint32_t i = vector->count; i > index; i--)
	{
		vector->data[i] = vector->data[i - 1];
	}

	vector->data[index] = data;
	vector->count++;
}

void kvector_remove(kvector *vector, uint32_t index)
{
	if (index >= vector->count)
	{
		return;
	}

	for (uint32_t i = index; i < vector->count - 1; i++)
	{
		vector->data[i] = vector->data[i + 1];
	}

	vector->count--;
}

void kvector_clear(kvector *vector)
{
	if (vector->size > 0)
	{
		for (uint32_t i = 0; i < vector->count; i++)
		{
			heap_kernel_dealloc(vector->data[i]);
		}

		heap_kernel_dealloc(vector->data);
		vector->size = 0;
		vector->count = 0;
	}
}