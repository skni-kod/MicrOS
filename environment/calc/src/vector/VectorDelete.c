#include "../vector.h"

/* 
    Delete from memory object of specified index in vector
*/
void VectorDelete(Vector *vector, const int index_)
{
    /* Calculate real index of reversed index */
    int index = index_;
    if (index < 0)
    {
        index = (vector->Size) + index;
    }

    if (vector == NULL || (index) > (vector->Size - 1))
    {
        return;
    }

    ObjectDelete(vector->Data[index]);

    void *tmp = 0;

    for (unsigned int i = index; i + 1 < vector->Size; i++)
    {
        vector->Data[i] = vector->Data[i + 1];
    }

    vector->Size -= 1;

    vector->Data = realloc(vector->Data, (sizeof(void *) * (vector->Size)));

    if (vector->Data == NULL)
        ReportError(-1);
}