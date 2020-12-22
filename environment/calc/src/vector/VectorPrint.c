#include "../vector.h"

/* Print content of vector */
void VectorPrint(Vector *vector)
{

    if (vector == NULL)
        return;

    if (vector->Size < 1)
    {
        printf("Vector is empty\n");
        return;
    }

    int tmp = (int)vector->Size;

    for (int i = 0; i < tmp; i++)
    {
        printf("%d", i + 1);
        ObjectPrint((Object *)(vector->Data[i]));
    }
}
