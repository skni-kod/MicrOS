#include "../vector.h"

/* Initialize new instance of object */
Object *ObjectInit(const void *data, const Type type)
{

    Object *obj = NULL;
    obj = malloc(sizeof(Object));
    if (obj != NULL)
    {
        obj->Type = type;
        obj->Size = ObjectDataSize(data, type);
        obj->Data = NULL;
        if (obj->Size > 0)
        {
            obj->Data = malloc(obj->Size);
            if (obj->Data != NULL)
                memcpy(obj->Data, data, obj->Size);
            else
                ReportError(-1);
        }
        return obj;
    }
    else
    {
        ReportError(-1);
    }
}
