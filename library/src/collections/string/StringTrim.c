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
        for (int i = (int)(object->Size - 2); i-- >= 0;)
        {
            if (((char *)object->Data)[i] == data)
                StringCut(object, (object->Size - 2), 1);
            else
            {
                StringCut(object, (object->Size - 2), 1);
                break;
            }
        }
    }

    if (mode == TrimLeading)
    {
        for (int i = 0; i <= (int)(object->Size - 2); i++)
        {
            if (((char *)object->Data)[i] == data)
                StringCut(object, 0, -1);
            else
            {
                StringCut(object, 0, -2);
                break;
            }
        }
    }
}