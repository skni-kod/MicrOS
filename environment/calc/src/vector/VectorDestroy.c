#include "../vector.h"

void VectorDestroy(Vector *vector)
{
    if (vector == NULL)
        return;

    if (vector->Data != NULL)
    {
        void *tmp = vector->Data;
        while (vector->Size > 0)
        {
            VectorDelete(vector, -1);
        }
    }
}