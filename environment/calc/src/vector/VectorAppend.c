#include "../vector.h"

/* Append data object to vector */
void VectorAppend(Vector *vector, const Object *object, const bool copy)
{

	if (object == NULL)
		return;

	if (vector == NULL)
		vector = VectorInit();

	Object *obj = NULL;

	/* Reallocate space for pointers */
	if (vector->Data == NULL)
		vector->Data = malloc(sizeof(void *));
	else
		vector->Data = realloc(vector->Data, (sizeof(void *) * (vector->Size + 1)));

	if (vector->Data != NULL)
	{
		/* Copy data object to new instance */
		if (copy)
			obj = ObjectInit(object->Data, object->Type);
		else /* Move pointer */
			obj = object;

		/* Allocate space for new data */
		vector->Data[vector->Size] = malloc(sizeof(void *));
		/* Copy pointer of new value stored in *obj */
		if (vector->Data[vector->Size] != 0)
			vector->Data[vector->Size++] = obj;
		else
			ReportError(1);
	}
	else
		ReportError(1);
}
