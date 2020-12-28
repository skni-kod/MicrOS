#include "../object.h"

int StringCut(Object *object, const unsigned int index, const int amount)
{
    if (!__objectCheckPointer(object) || !__objectCheckPointer(object->Data))
        return 0;

    if (object->Type != String || object->Size < 1)
    {
        __objectReportError(-2);
        return 0;
    }

    if (amount == 0 || abs(amount) >= (object->Size - 1))
    {
        __objectReportError(-6);
        return 0;
    }

    unsigned int rangeBegin = 0, rangeEnd = 0;
    if (amount < 0)
    {
        rangeBegin = (index - abs(amount) + 1) > 0 ? (index - abs(amount) + 1) : 0;
        rangeEnd = index > (object->Size - 2) ? (object->Size - 2) : index;
    }
    if (amount > 0)
    {
        rangeBegin = index > (object->Size - 2) ? (object->Size - 2) : index;
        rangeEnd = (index + amount - 1) > (object->Size - 2) ? (object->Size - 2) : (index + amount - 1);
    }

    Object *tmp = ObjectInit("");
    char tmpChar[2] = "";
    int count = 0;
    for (size_t i = 0; i < (object->Size - 1); i++)
    {
        if (!(i >= rangeBegin && i <= rangeEnd))
        {
            tmpChar[0] = ((char *)object->Data)[i];
            StringAppend(tmp, tmpChar);
        }
        else
            count++;
    }

    ObjectSet(object, (char *)tmp->Data);
    ObjectDelete(tmp);
    return count;
}
