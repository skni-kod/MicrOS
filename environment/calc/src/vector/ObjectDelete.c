#include "../vector.h"

void ObjectDelete(Object *object)
{
    free((void *)object->Data);
    free(object);
}