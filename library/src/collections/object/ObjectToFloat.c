#include "../object.h"

//TODO
//Catching value overflow

bool ObjectToFloat(Object *object)
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

    if (object->Type == Float)
        return true;

    if (object->Type == Double)
    {
        double val = *(double *)object->Data;
        object->Type = Float;
        object->Size = sizeof(float);
        object->Data = realloc(object->Data, sizeof(float));
        float tmp = (float)val;
        memcpy(object->Data, &tmp, sizeof(float));
    }

    if (object->Type == Int)
    {
        int val = *(int *)object->Data;
        object->Type = Float;
        object->Size = sizeof(float);
        object->Data = realloc(object->Data, sizeof(float));
        float tmp = (float)val;
        memcpy(object->Data, &tmp, sizeof(float));
    }

    if (object->Type == UInt)
    {
        unsigned int val = *(unsigned int *)object->Data;
        object->Type = Float;
        object->Size = sizeof(float);
        object->Data = realloc(object->Data, sizeof(float));
        float tmp = (float)val;
        memcpy(object->Data, &tmp, sizeof(float));
    }

    if (object->Type == Long)
    {
        long val = *(long *)object->Data;
        object->Type = Float;
        object->Size = sizeof(float);
        object->Data = realloc(object->Data, sizeof(float));
        float tmp = (float)val;
        memcpy(object->Data, &tmp, sizeof(float));
    }

    if (object->Type == ULong)
    {
        unsigned long val = *(unsigned long *)object->Data;
        object->Type = Float;
        object->Size = sizeof(float);
        object->Data = realloc(object->Data, sizeof(float));
        float tmp = (float)val;
        memcpy(object->Data, &tmp, sizeof(float));
    }

    if (object->Type == String)
    {
        float tmp = 0;
        if (!sscanf(object->Data, "%f", &tmp))
        {
            __objectReportError(-2);
            return false;
        }
        object->Type = Float;
        object->Size = sizeof(float);
        object->Data = realloc(object->Data, sizeof(float));
        memcpy(object->Data, &tmp, sizeof(float));
        return true;
    }

    __objectReportError(-4);
    return false;
}

bool ObjectToDouble(Object *object)
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

    if (object->Type == Double)
        return true;

    if (object->Type == Float)
    {
        float val = *(float *)object->Data;
        object->Type = Double;
        object->Size = sizeof(double);
        object->Data = realloc(object->Data, sizeof(double));
        double tmp = (double)val;
        memcpy(object->Data, &tmp, sizeof(double));
    }

    if (object->Type == Int)
    {
        int val = *(int *)object->Data;
        object->Type = Double;
        object->Size = sizeof(double);
        object->Data = realloc(object->Data, sizeof(double));
        double tmp = (double)val;
        memcpy(object->Data, &tmp, sizeof(double));
    }

    if (object->Type == UInt)
    {
        unsigned int val = *(unsigned int *)object->Data;
        object->Type = Double;
        object->Size = sizeof(double);
        object->Data = realloc(object->Data, sizeof(double));
        double tmp = (double)val;
        memcpy(object->Data, &tmp, sizeof(double));
    }

    if (object->Type == Long)
    {
        long val = *(long *)object->Data;
        object->Type = Double;
        object->Size = sizeof(double);
        object->Data = realloc(object->Data, sizeof(double));
        double tmp = (double)val;
        memcpy(object->Data, &tmp, sizeof(double));
    }

    if (object->Type == ULong)
    {
        unsigned long val = *(unsigned long *)object->Data;
        object->Type = Double;
        object->Size = sizeof(double);
        object->Data = realloc(object->Data, sizeof(double));
        double tmp = (double)val;
        memcpy(object->Data, &tmp, sizeof(double));
    }

    if (object->Type == String)
    {
        double tmp = 0;
        if (!sscanf(object->Data, "%lf", &tmp))
        {
            __objectReportError(-2);
            return false;
        }
        object->Type = Double;
        object->Size = sizeof(double);
        object->Data = realloc(object->Data, sizeof(double));
        memcpy(object->Data, &tmp, sizeof(double));
        return true;
    }

    __objectReportError(-4);
    return false;
}