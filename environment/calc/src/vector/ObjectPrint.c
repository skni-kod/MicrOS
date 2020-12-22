#include "../vector.h"

void ObjectPrint(Object *object)
{
    if (object == NULL)
        return;
    if (object->Data != NULL)
    {
        switch (object->Type)
        {
        case String:
            printf("(String){%lu}->%s\n", (size_t)(object->Size), (char *)(object->Data));
            break;
        case Int:
            printf("(Int)->%d\n", *(int *)(object->Data));
            break;
        case Double:
            printf("(Double)->%lf\n", *(double *)(object->Data));
            break;
        case Pointer:
            printf("(Pointer)->%p\n", (void *)(object->Data));
            break;
        default:
            printf("I don't know how to print it ;(\n");
            break;
        };
    }
}