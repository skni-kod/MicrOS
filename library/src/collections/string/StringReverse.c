#include "../object.h"

bool StringReverse(Object *object)
{
    if (!__objectCheckPointer(object) || !__objectCheckPointer(object->Data))
        return 0;

    if (object->Size < 1)
    {
        __objectReportError(-6);
        return 0;
    }

    if (object->Type != String)
    {
        __objectReportError(-6);
        return 0;
    }

    Object *tmp = ObjectInit("");
    char tmpChar[2] = "";
    for (int i = (object->Size - 2); i >= 0; i--)
    {
        tmpChar[0] = ((char *)object->Data)[i];
        StringAppend(tmp, tmpChar);
    }
    object->Size = tmp->Size;
    free(object->Data);
    object->Data = tmp->Data;
    tmp->Data = NULL;
    free(tmp);
}