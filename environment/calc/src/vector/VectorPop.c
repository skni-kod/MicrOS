#include "../vector.h"

/* 
	Removes last element from vector
	Returns pointer to last object in vector

*/
Object *VectorPop(Vector *vector)
{

	if (vector == NULL || vector->Size == 0)
	{
		ReportError(-3);
		return NULL;
	}

	vector->Size -= 1;

	if (vector->Data != NULL)
	{
		void *tmp = vector->Data[vector->Size];
		if (vector->Size == 0)
		{
			vector->Data = NULL;
			return tmp;
		}

		vector->Data = realloc(vector->Data, (sizeof(void *) * (vector->Size)));
		if (vector->Data != NULL)
		{
			return tmp;
		}
		else
			ReportError(-1);
	}
}