#include "../object.h"

bool ObjectToString(Object *object)
{
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

    if (object->Type == Pointer)
    {
        char *tmp = malloc(sizeof(char) * 10);
        __objectCheckPointer(tmp);
        sprintf(tmp, "%p\0", (void *)object->Data);
        free(object->Data);
        object->Data = tmp;
        object->Size = 10;
        object->Type = String;
        return true;
    }

    if (object->Type == Int)
    {
        Object *tmp = ObjectInit("");

        int value = *((int *)object->Data);
        bool valueSign = value >= 0 ? true : false;

        if (!valueSign)
            value *= -1;
        char tmpChar[2] = {'\0', '\0'};
        while (true)
        {
            *tmpChar = '0' + (value % 10);
            StringAppend(tmp, tmpChar);
            value = value / 10;
            if (value == 0)
                break;
        }
        //Leading sign
        if (!valueSign)
            StringAppend(tmp, "-");
        StringReverse(tmp);

        ObjectSet(object, (char *)tmp->Data);
        ObjectDelete(tmp);
        return true;
    }

    if (object->Type == UInt)
    {
        Object *tmp = ObjectInit("");

        unsigned int value = *((unsigned int *)object->Data);
        bool valueSign = value >= 0 ? true : false;

        if (!valueSign)
            value *= -1;
        char *tmpChar = malloc(sizeof(char) * 2);
        *(char *)(tmpChar + 1) = '\0';
        while (true)
        {
            *tmpChar = '0' + (value % 10);
            StringAppend(tmp, tmpChar);
            value = value / 10;
            if (value == 0)
                break;
        }

        free(tmpChar);
        tmpChar = NULL;
        //Leading sign
        if (!valueSign)
            StringAppend(tmp, "-");
        StringReverse(tmp);

        ObjectSet(object, (char *)tmp->Data);
        ObjectDelete(tmp);
        return true;
    }

    if (object->Type == Long)
    {
        Object *tmp = ObjectInit("");

        long value = *((long *)object->Data);
        bool valueSign = value >= 0 ? true : false;

        if (!valueSign)
            value *= -1;
        char tmpChar[2] = {'\0', '\0'};
        while (true)
        {
            *tmpChar = '0' + (value % 10);
            StringAppend(tmp, tmpChar);
            value = value / 10;
            if (value == 0)
                break;
        }
        //Leading sign
        if (!valueSign)
            StringAppend(tmp, "-");
        StringReverse(tmp);

        ObjectSet(object, (char *)tmp->Data);
        ObjectDelete(tmp);
        return true;
    }

    if (object->Type == ULong)
    {
        Object *tmp = ObjectInit("");

        int value = *((int *)object->Data);
        bool valueSign = value >= 0 ? true : false;

        if (!valueSign)
            value *= -1;
        //Temporary char
        char tmpChar[2] = {'\0', '\0'};
        while (true)
        {
            *tmpChar = '0' + (value % 10);
            StringAppend(tmp, tmpChar);
            value = value / 10;
            if (value == 0)
                break;
        }
        //Leading sign
        if (!valueSign)
            StringAppend(tmp, "-");
        StringReverse(tmp);

        ObjectSet(object, (char *)tmp->Data);
        ObjectDelete(tmp);
        return true;
    }

    if (object->Type == Double)
    {
        Object *tmp = ObjectInit("");

        double value = *((double *)object->Data);

        bool valueSign = value >= 0 ? true : false;

        if (!valueSign)
            value *= -1;

        unsigned long tmpVar = (unsigned long)value;
        Object *intPart = ObjectInit(&tmpVar);
        tmpVar = (unsigned long)((value - (double)tmpVar) * 10000000000);
        Object *fracPart = ObjectInit(&tmpVar);

        if (!valueSign)
            StringAppend(tmp, "-");

        ObjectToString(intPart);
        StringAppend(tmp, ((char *)intPart->Data));

        StringAppend(tmp, ".");

        ObjectToString(fracPart);
        StringAppend(tmp, ((char *)fracPart->Data));

        ObjectSet(object, (char *)tmp->Data);
        ObjectDelete(tmp);
        StringTrim(object, '0', TrimTailing);
        return true;
    }

    if (object->Type == Float)
    {
        Object *tmp = ObjectInit("");

        float value = *((float *)object->Data);

        bool valueSign = value >= 0 ? true : false;

        if (!valueSign)
            value *= -1;

        unsigned int tmpVar = (unsigned int)value;
        Object *intPart = ObjectInit(&tmpVar);
        tmpVar = (unsigned int)((value - (float)tmpVar) * 1000000);
        Object *fracPart = ObjectInit(&tmpVar);

        if (!valueSign)
            StringAppend(tmp, "-");

        ObjectToString(intPart);
        StringAppend(tmp, ((char *)intPart->Data));

        StringAppend(tmp, ".");

        ObjectToString(fracPart);
        StringAppend(tmp, ((char *)fracPart->Data));

        ObjectSet(object, (char *)tmp->Data);
        ObjectDelete(tmp);
        StringTrim(object, '0', TrimTailing);
        return true;
    }
}
