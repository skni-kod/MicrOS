#include "../object.h"

bool ObjectToString(Object *object)
{
    //TODO
    if (!__objectCheckPointer(object) || !__objectCheckPointer(object->Data))
        return false;

    if (object->Size < 1)
    {
        __objectReportError(-6);
        return false;
    }

    if (object->Type == String)
        return true;

    if (object->Type == None)
    {
        __objectReportError(-4);
        return false;
    }

    if (object->Type == Int)
    {
        int value = *((int *)object->Data);
        //Sign of value
        bool valSign = value >= 0 ? true : false;
        Object *tmp = ObjectInit("");

        //Absolute value
        if (!valSign)
            value *= -1;

        while (true)
        {
            //Temporary char
            char tmpChar[2] = {'0' + (value % 10), '\0'};
            StringAppend(tmp, tmpChar);
            value = value / 10;
            if (value == 0)
                break;
        }

        //Leading sign
        if (!valSign)
            StringAppend(tmp, "-");

        StringReverse(tmp);
        free(object->Data);
        object->Type = String;
        object->Size = tmp->Size;
        object->Data = tmp->Data;
        tmp->Data = NULL;
        free(tmp);
        return;
    }
}