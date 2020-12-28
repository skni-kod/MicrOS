#include "../object.h"

int StringTrim(Object *object, const char data, TrimMode mode)
{
    if (!__objectCheckPointer(object) || !__objectCheckPointer(object->Data))
        return 0;

    if (object->Type != String || object->Size < 1)
    {
        __objectReportError(-2);
        return 0;
    }

    if (mode == TrimTailing)
    {
        int count = 0;
        while (object->Size - 1)
        {
            if (((char *)object->Data)[StringLast(object)] == data)
            {
                StringCut(object, StringLast(object), 1);
                count++;
            }
            else
                break;
        }
        return count;
    }

    if (mode == TrimLeading)
    {
        int count = 0;
        while (object->Size - 1)
        {
            if (((char *)object->Data)[0] == data)
            {
                StringCut(object, 0, 1);
                count++;
            }
            else
                break;
        }
        return count;
    }
}