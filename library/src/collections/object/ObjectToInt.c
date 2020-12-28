#include "../object.h"

bool ObjectToInt(Object *object)
{
    if (!__objectCheckPointer(object) || !__objectCheckPointer(object->Data))
        return false;

    if (object->Size < 1)
    {
        __objectReportError(-6);
        return false;
    }

    if (object->Type == None)
    {
        __objectReportError(-4);
        return false;
    }

    if (object->Type == Int)
        return true;

    if (object->Type == UInt)
    {
        unsigned int val = *(unsigned int *)object->Data;
        if (val > INT_MAX)
        {
            __objectReportError(-7);
            return false;
        }
        object->Type = Int;
        int tmp = (int)val;
        memcpy(object->Data, &tmp, sizeof(int));
        return true;
    }

    if (object->Type == Long)
    {
        long val = *(long *)object->Data;
        if (val > INT_MAX || val < INT_MIN)
        {
            __objectReportError(-7);
            return false;
        }

        object->Type = Int;
        object->Size = sizeof(int);
        int tmp = (int)val;
        object->Data = realloc(object->Data, sizeof(int));
        memcpy(object->Data, &tmp, sizeof(int));
        return true;
    }

    if (object->Type == ULong)
    {
        unsigned long val = *(unsigned long *)object->Data;
        if (val > INT_MAX)
        {
            __objectReportError(-7);
            return false;
        }

        object->Type = Int;
        object->Size = sizeof(int);
        int tmp = (int)val;
        object->Data = realloc(object->Data, sizeof(int));
        memcpy(object->Data, &tmp, sizeof(int));
        return true;
    }

    if (object->Type == Double)
    {
        double val = *(double *)object->Data;
        if (val > INT_MAX || val < INT_MIN)
        {
            __objectReportError(-7);
            return false;
        }

        object->Type = Int;
        object->Size = sizeof(int);
        int tmp = (int)val;
        object->Data = realloc(object->Data, sizeof(int));
        memcpy(object->Data, &tmp, sizeof(int));
        return true;
    }

    if (object->Type == Float)
    {
        float val = *(float *)object->Data;
        if (val > INT_MAX || val < INT_MIN)
        {
            __objectReportError(-7);
            return false;
        }

        object->Type = Int;
        object->Size = sizeof(int);
        int tmp = (int)val;
        object->Data = realloc(object->Data, sizeof(int));
        memcpy(object->Data, &tmp, sizeof(int));
        return true;
    }

    if (object->Type == String)
    {
        int tmp = 0;
        if (!sscanf(object->Data, "%d", &tmp))
        {
            __objectReportError(-2);
            return false;
        }
        object->Type = Int;
        object->Size = sizeof(int);
        object->Data = realloc(object->Data, sizeof(int));
        memcpy(object->Data, &tmp, sizeof(int));
        return true;
    }

    __objectReportError(-4);
    return false;
}

bool ObjectToUInt(Object *object)
{
    if (!__objectCheckPointer(object) || !__objectCheckPointer(object->Data))
        return false;

    if (object->Size < 1)
    {
        __objectReportError(-6);
        return false;
    }

    if (object->Type == None)
    {
        __objectReportError(-4);
        return false;
    }

    if (object->Type == UInt)
        return true;

    if (object->Type == Int)
    {
        int val = *(int *)object->Data;
        if (val < 0)
        {
            __objectReportError(-8);
            return false;
        }
        object->Type = UInt;
        unsigned int tmp = (unsigned int)val;
        memcpy(object->Data, &tmp, sizeof(int));
        return true;
    }

    if (object->Type == Long)
    {
        long val = *(long *)object->Data;
        if (val > UINT_MAX)
        {
            __objectReportError(-7);
            return false;
        }
        if (val < 0)
        {
            __objectReportError(-8);
            return false;
        }

        object->Type = UInt;
        object->Size = sizeof(int);
        unsigned int tmp = (unsigned int)val;
        object->Data = realloc(object->Data, sizeof(int));
        memcpy(object->Data, &tmp, sizeof(int));
        return true;
    }

    if (object->Type == ULong)
    {
        unsigned long val = *(unsigned long *)object->Data;
        if (val > UINT_MAX)
        {
            __objectReportError(-7);
            return false;
        }

        object->Type = UInt;
        object->Size = sizeof(int);
        unsigned int tmp = (unsigned int)val;
        object->Data = realloc(object->Data, sizeof(int));
        memcpy(object->Data, &tmp, sizeof(int));
        return true;
    }

    if (object->Type == Double)
    {
        double val = *(double *)object->Data;
        if (val > UINT_MAX)
        {
            __objectReportError(-7);
            return false;
        }
        if (val < 0)
        {
            __objectReportError(-8);
            return false;
        }

        object->Type = UInt;
        object->Size = sizeof(int);
        unsigned int tmp = (unsigned int)val;
        object->Data = realloc(object->Data, sizeof(int));
        memcpy(object->Data, &tmp, sizeof(int));
        return true;
    }

    if (object->Type == Float)
    {
        float val = *(float *)object->Data;
        if (val > UINT_MAX)
        {
            __objectReportError(-7);
            return false;
        }
        if (val < 0)
        {
            __objectReportError(-8);
            return false;
        }

        object->Type = UInt;
        object->Size = sizeof(int);
        unsigned int tmp = (unsigned int)val;
        object->Data = realloc(object->Data, sizeof(int));
        memcpy(object->Data, &tmp, sizeof(int));
        return true;
    }

    if (object->Type == String)
    {
        unsigned int tmp = 0;
        if (!sscanf(object->Data, "%u", &tmp))
        {
            __objectReportError(-2);
            return false;
        }
        object->Type = UInt;
        object->Size = sizeof(int);
        object->Data = realloc(object->Data, sizeof(int));
        memcpy(object->Data, &tmp, sizeof(int));
        return true;
    }

    __objectReportError(-4);
    return false;
}