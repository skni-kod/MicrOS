#include "vector.h"

void vector_init(vector *vector)
{
	vector->data = 0;
	vector->size = 0;
	vector->count = 0;
}

void vector_add(vector *vector, void *data)
{
	vector_insert(vector, data, vector->count);
}

void vector_insert(vector *vector, void *data, uint32_t index)
{
	if (vector->size == 0)
	{
		vector->size = 8;
		vector->data = malloc(sizeof(void *) * vector->size);
	}

	if (vector->count + 1 > vector->size)
	{
		vector->size *= 2;
		vector->data = realloc(vector->data, sizeof(void *) * vector->size);
	}

	for (uint32_t i = vector->count; i > index; i--)
	{
		vector->data[i] = vector->data[i - 1];
	}

	vector->data[index] = data;
	vector->count++;
}

void vector_remove(vector *vector, uint32_t index)
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

void vector_clear(vector *vector)
{
	if (vector->size > 0)
	{
		for (uint32_t i = 0; i < vector->count; i++)
		{
			free(vector->data[i]);
		}

		free(vector->data);
		vector->size = 0;
		vector->count = 0;
	}
}