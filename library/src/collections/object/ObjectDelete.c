#include "../object.h"

bool ObjectDelete(Object *object)
{
    if (object == NULL)
    {
        __objectReportError(-3);
        return false;
    }
    if (object->Data != NULL)
    {
        free(object->Data);
        object->Data = NULL;
        object->Size = 0;
        object->Type = None;
    }

    free(object);
    object = NULL;
    return true;
}