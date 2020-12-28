#include "../object.h"

bool ObjectToLong(Object *object)
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

    if (object->Type == Long)
        return true;

    if (object->Type == ULong)
    {
        unsigned long val = *(unsigned long *)object->Data;
        if (val > LONG_MAX)
        {
            __objectReportError(-7);
            return false;
        }
        object->Type = Long;
        long tmp = (long)val;
        memcpy(object->Data, &tmp, sizeof(long));
        return true;
    }

    if (object->Type == Int)
    {
        long val = *(int *)object->Data;
        object->Type = Long;
        object->Size = sizeof(long);
        long tmp = (long)val;
        object->Data = realloc(object->Data, sizeof(long));
        memcpy(object->Data, &tmp, sizeof(long));
        return true;
    }

    if (object->Type == UInt)
    {
        unsigned int val = *(unsigned int *)object->Data;

        object->Type = Long;
        object->Size = sizeof(long);
        long tmp = (long)val;
        object->Data = realloc(object->Data, sizeof(long));
        memcpy(object->Data, &tmp, sizeof(long));
        return true;
    }

    if (object->Type == Double)
    {
        double val = *(double *)object->Data;
        if (val > LONG_MAX || val < LONG_MIN)
        {
            __objectReportError(-7);
            return false;
        }

        object->Type = Long;
        object->Size = sizeof(long);
        long tmp = (long)val;
        object->Data = realloc(object->Data, sizeof(long));
        memcpy(object->Data, &tmp, sizeof(long));
        return true;
    }

    if (object->Type == Float)
    {
        float val = *(float *)object->Data;
        if (val > LONG_MAX || val < LONG_MIN)
        {
            __objectReportError(-7);
            return false;
        }

        object->Type = Long;
        object->Size = sizeof(long);
        long tmp = (long)val;
        object->Data = realloc(object->Data, sizeof(long));
        memcpy(object->Data, &tmp, sizeof(long));
        return true;
    }

    if (object->Type == String)
    {
        long tmp = 0;
        if (!sscanf(object->Data, "%li", &tmp))
        {
            __objectReportError(-2);
            return false;
        }
        object->Type = Long;
        object->Size = sizeof(long);
        object->Data = realloc(object->Data, sizeof(long));
        memcpy(object->Data, &tmp, sizeof(long));
        return true;
    }

    __objectReportError(-4);
    return false;
}

bool ObjectToULong(Object *object)
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

    if (object->Type == ULong)
        return true;

    if (object->Type == Long)
    {
        long val = *(long *)object->Data;
        if (val < 0)
        {
            __objectReportError(-8);
            return false;
        }
        object->Type = ULong;
        unsigned long tmp = (unsigned long)val;
        memcpy(object->Data, &tmp, sizeof(long));
        return true;
    }

    if (object->Type == Int)
    {
        int val = *(int *)object->Data;
        if (val < 0)
        {
            __objectReportError(-8);
            return false;
        }

        object->Type = ULong;
        object->Size = sizeof(long);
        unsigned long tmp = (unsigned long)val;
        object->Data = realloc(object->Data, sizeof(long));
        memcpy(object->Data, &tmp, sizeof(long));
        return true;
    }

    if (object->Type == UInt)
    {
        unsigned int val = *(unsigned int *)object->Data;

        object->Type = UInt;
        object->Size = sizeof(long);
        unsigned int tmp = (unsigned long)val;
        object->Data = realloc(object->Data, sizeof(long));
        memcpy(object->Data, &tmp, sizeof(long));
        return true;
    }

    if (object->Type == Double)
    {
        double val = *(double *)object->Data;
        if (val > ULONG_MAX)
        {
            __objectReportError(-7);
            return false;
        }
        if (val < 0)
        {
            __objectReportError(-8);
            return false;
        }

        object->Type = ULong;
        object->Size = sizeof(long);
        unsigned int tmp = (unsigned long)val;
        object->Data = realloc(object->Data, sizeof(long));
        memcpy(object->Data, &tmp, sizeof(long));
        return true;
    }

    if (object->Type == Float)
    {
        float val = *(float *)object->Data;
        if (val > ULONG_MAX)
        {
            __objectReportError(-7);
            return false;
        }
        if (val < 0)
        {
            __objectReportError(-8);
            return false;
        }

        object->Type = ULong;
        object->Size = sizeof(long);
        unsigned int tmp = (unsigned long)val;
        object->Data = realloc(object->Data, sizeof(long));
        memcpy(object->Data, &tmp, sizeof(long));
        return true;
    }

    if (object->Type == String)
    {
        unsigned long tmp = 0;
        if (!sscanf(object->Data, "%lu", &tmp))
        {
            __objectReportError(-2);
            return false;
        }
        object->Type = ULong;
        object->Size = sizeof(long);
        object->Data = realloc(object->Data, sizeof(long));
        memcpy(object->Data, &tmp, sizeof(long));
        return true;
    }

    __objectReportError(-4);
    return false;
}