#include "../object.h"

int StringLast(Object *object)
{
    if (!__objectCheckPointer(object) || !__objectCheckPointer(object->Data))
        return -1;

    if (object->Size < 1)
    {
        __objectReportError(-6);
        return -1;
    }

    if (object->Type != String)
    {
        __objectReportError(-6);
        return -1;
    }

    return (int)(object->Size - 2);
}