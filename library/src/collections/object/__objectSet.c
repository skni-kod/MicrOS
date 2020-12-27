#include "../object.h"

bool __objectSetFloat(Object *object, float data)
{
    if (!__objectCheckPointer(object))
    {
        __objectReportError(-3);
        return false;
    }
    free(object->Data);
    object->Data = NULL;
    object->Data = malloc(sizeof(float));
    if (!__objectCheckPointer(object->Data))
    {
        __objectReportError(-1);
        return false;
    }
    object->Type = Float;
    object->Size = sizeof(float);
    *(float *)object->Data = data;
    return true;
}

bool __objectSetFloatPointer(Object *object, float *data)
{
    if (!__objectCheckPointer(object))
    {
        __objectReportError(-3);
        return false;
    }

    if (!__objectCheckPointer(data))
    {
        __objectReportError(-3);
        return false;
    }

    free(object->Data);
    object->Data = NULL;
    object->Data = malloc(sizeof(float));
    if (!__objectCheckPointer(object->Data))
    {
        __objectReportError(-1);
        return false;
    }
    object->Type = Float;
    object->Size = sizeof(float);
    memcpy(object->Data, data, sizeof(float));
    return true;
}

bool __objectSetDouble(Object *object, double data)
{
    if (!__objectCheckPointer(object))
    {
        __objectReportError(-3);
        return false;
    }
    free(object->Data);
    object->Data = NULL;
    object->Data = malloc(sizeof(double));
    if (!__objectCheckPointer(object->Data))
    {
        __objectReportError(-1);
        return false;
    }
    object->Type = Double;
    object->Size = sizeof(double);
    *(double *)object->Data = data;
    return true;
}

bool __objectSetDoublePointer(Object *object, double *data)
{
    if (!__objectCheckPointer(object))
    {
        __objectReportError(-3);
        return false;
    }

    if (!__objectCheckPointer(data))
    {
        __objectReportError(-3);
        return false;
    }

    free(object->Data);
    object->Data = NULL;
    object->Data = malloc(sizeof(double));
    if (!__objectCheckPointer(object->Data))
    {
        __objectReportError(-1);
        return false;
    }
    object->Type = Double;
    object->Size = sizeof(double);
    memcpy(object->Data, data, sizeof(double));
    return true;
}

bool __objectSetInt(Object *object, int data)
{
    if (!__objectCheckPointer(object))
    {
        __objectReportError(-3);
        return false;
    }
    free(object->Data);
    object->Data = NULL;
    object->Data = malloc(sizeof(int));
    if (!__objectCheckPointer(object->Data))
    {
        __objectReportError(-1);
        return false;
    }
    object->Type = Int;
    object->Size = sizeof(int);
    *(int *)object->Data = data;
    return true;
}

bool __objectSetIntPointer(Object *object, int *data)
{
    if (!__objectCheckPointer(object))
    {
        __objectReportError(-3);
        return false;
    }

    if (!__objectCheckPointer(data))
    {
        __objectReportError(-3);
        return false;
    }

    free(object->Data);
    object->Data = NULL;
    object->Data = malloc(sizeof(int));
    if (!__objectCheckPointer(object->Data))
    {
        __objectReportError(-1);
        return false;
    }
    object->Type = Int;
    object->Size = sizeof(int);
    memcpy(object->Data, data, sizeof(int));
    return true;
}

bool __objectSetUInt(Object *object, unsigned int data)
{
    if (!__objectCheckPointer(object))
    {
        __objectReportError(-3);
        return false;
    }
    free(object->Data);
    object->Data = NULL;
    object->Data = malloc(sizeof(unsigned int));
    if (!__objectCheckPointer(object->Data))
    {
        __objectReportError(-1);
        return false;
    }
    object->Type = UInt;
    object->Size = sizeof(unsigned int);
    *(unsigned int *)object->Data = data;
    return true;
}

bool __objectSetUIntPointer(Object *object, unsigned int *data)
{
    if (!__objectCheckPointer(object))
    {
        __objectReportError(-3);
        return false;
    }

    if (!__objectCheckPointer(data))
    {
        __objectReportError(-3);
        return false;
    }

    free(object->Data);
    object->Data = NULL;
    object->Data = malloc(sizeof(unsigned int));
    if (!__objectCheckPointer(object->Data))
    {
        __objectReportError(-1);
        return false;
    }
    object->Type = UInt;
    object->Size = sizeof(unsigned int);
    memcpy(object->Data, data, sizeof(unsigned int));
    return true;
}

bool __objectSetLong(Object *object, long data)
{
    if (!__objectCheckPointer(object))
    {
        __objectReportError(-3);
        return false;
    }
    free(object->Data);
    object->Data = NULL;
    object->Data = malloc(sizeof(long));
    if (!__objectCheckPointer(object->Data))
    {
        __objectReportError(-1);
        return false;
    }
    object->Type = Long;
    object->Size = sizeof(long);
    *(long *)object->Data = data;
    return true;
}

bool __objectSetLongPointer(Object *object, long *data)
{
    if (!__objectCheckPointer(object))
    {
        __objectReportError(-3);
        return false;
    }

    if (!__objectCheckPointer(data))
    {
        __objectReportError(-3);
        return false;
    }

    free(object->Data);
    object->Data = NULL;
    object->Data = malloc(sizeof(long));
    if (!__objectCheckPointer(object->Data))
    {
        __objectReportError(-1);
        return false;
    }
    object->Type = Long;
    object->Size = sizeof(long);
    memcpy(object->Data, data, sizeof(long));
    return true;
}

bool __objectSetULong(Object *object, unsigned long data)
{
    if (!__objectCheckPointer(object))
    {
        __objectReportError(-3);
        return false;
    }
    free(object->Data);
    object->Data = NULL;
    object->Data = malloc(sizeof(unsigned long));
    if (!__objectCheckPointer(object->Data))
    {
        __objectReportError(-1);
        return false;
    }
    object->Type = ULong;
    object->Size = sizeof(unsigned long);
    *(unsigned long *)object->Data = data;
    return true;
}

bool __objectSetULongPointer(Object *object, unsigned long *data)
{
    if (!__objectCheckPointer(object))
    {
        __objectReportError(-3);
        return false;
    }

    if (!__objectCheckPointer(data))
    {
        __objectReportError(-3);
        return false;
    }

    free(object->Data);
    object->Data = NULL;
    object->Data = malloc(sizeof(unsigned long));
    if (!__objectCheckPointer(object->Data))
    {
        __objectReportError(-1);
        return false;
    }
    object->Type = ULong;
    object->Size = sizeof(unsigned long);
    memcpy(object->Data, data, sizeof(unsigned long));
    return true;
}

bool __objectSetChar(Object *object, char data)
{
    if (!__objectCheckPointer(object))
    {
        __objectReportError(-3);
        return false;
    }
    free(object->Data);
    object->Data = NULL;
    object->Data = malloc(sizeof(char) * 2);
    if (!__objectCheckPointer(object->Data))
    {
        __objectReportError(-1);
        return false;
    }
    object->Type = String;
    object->Size = sizeof(char) * 2;
    *(char *)(object->Data) = data;
    *(char *)(object->Data + 1) = '\0';
    return true;
}

bool __objectSetCharPointer(Object *object, char *data)
{
    if (!__objectCheckPointer(object))
    {
        __objectReportError(-3);
        return false;
    }

    if (!__objectCheckPointer(data))
    {
        __objectReportError(-3);
        return false;
    }

    size_t stringSize = strlen(data) + 1;

    free(object->Data);
    object->Data = NULL;
    object->Data = malloc(sizeof(char) * stringSize);
    if (!__objectCheckPointer(object->Data))
    {
        __objectReportError(-1);
        return false;
    }
    object->Type = String;
    object->Size = sizeof(char) * stringSize;
    memcpy(object->Data, data, sizeof(char) * stringSize);
    return true;
}

bool __objectSetPointer(Object *object, void *data)
{
    if (!__objectCheckPointer(object))
    {
        __objectReportError(-3);
        return false;
    }
    free(object->Data);
    object->Data = NULL;
    object->Data = malloc(sizeof(void *));
    if (!__objectCheckPointer(object->Data))
    {
        __objectReportError(-1);
        return false;
    }
    object->Type = Pointer;
    object->Size = sizeof(void *);
    object->Data = data;
    return true;
}