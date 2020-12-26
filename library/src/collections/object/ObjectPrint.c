#include "../object.h"

void ObjectPrint(Object *object)
{
    if (object == NULL || object->Data == NULL)
    {
        __objectReportError(-3);
        return;
    }

    switch (object->Type)
    {
    case String:
        printf("(String){%zu}->%s\n", (size_t)(object->Size), (char *)(object->Data));
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