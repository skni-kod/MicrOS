#include "../object.h"

Object *__objectInit(size_t size, Type type)
{
    Object *obj = NULL;
    obj = malloc(sizeof(Object));
    if (obj != NULL)
    {
        obj->Type = type;
        obj->Size = size;
        obj->Data = NULL;
        obj->Data = malloc(size);
        if (obj->Data != NULL)
            return obj;
        else
            __objectReportError(-1);
    }
    else
        __objectReportError(-1);
}

bool __objectCheckPointer(void *data)
{
    if (data != NULL)
        return true;
    else
    {
        __objectReportError(-3);
        return false;
    }
}

Object *__objectInitFloat(float data)
{
    Object *obj = __objectInit(sizeof(float), Float);
    *(float *)(obj->Data) = data;
    return obj;
}

Object *__objectInitFloatPointer(float *data)
{
    size_t typeSize = sizeof(float);
    if (!__objectCheckPointer(data))
        return NULL;

    Object *obj = __objectInit(typeSize, Float);
    memcpy(obj->Data, data, typeSize);
    return obj;
}

Object *__objectInitDouble(double data)
{
    Object *obj = __objectInit(sizeof(double), Double);
    *(double *)(obj->Data) = data;
    return obj;
}

Object *__objectInitDoublePointer(double *data)
{
    size_t typeSize = sizeof(double);
    if (!__objectCheckPointer(data))
        return NULL;

    Object *obj = __objectInit(typeSize, Double);
    memcpy(obj->Data, data, typeSize);
    return obj;
}

Object *__objectInitInt(int data)
{
    Object *obj = __objectInit(sizeof(int), Int);
    *(int *)(obj->Data) = data;
    return obj;
}

Object *__objectInitIntPointer(int *data)
{
    size_t typeSize = sizeof(int);
    if (!__objectCheckPointer(data))
        return NULL;

    Object *obj = __objectInit(typeSize, Int);
    memcpy(obj->Data, data, typeSize);
    return obj;
}

Object *__objectInitUInt(unsigned int data)
{
    Object *obj = __objectInit(sizeof(unsigned int), UInt);
    *(unsigned int *)(obj->Data) = data;
    return obj;
}

Object *__objectInitUIntPointer(unsigned int *data)
{
    size_t typeSize = sizeof(unsigned int);
    if (!__objectCheckPointer(data))
        return NULL;

    Object *obj = __objectInit(typeSize, UInt);
    memcpy(obj->Data, data, typeSize);
    return obj;
}

Object *__objectInitLong(long data)
{
    Object *obj = __objectInit(sizeof(long), Long);
    *(long *)(obj->Data) = data;
    return obj;
}

Object *__objectInitLongPointer(long *data)
{
    size_t typeSize = sizeof(long);
    if (!__objectCheckPointer(data))
        return NULL;

    Object *obj = __objectInit(typeSize, Long);
    memcpy(obj->Data, data, typeSize);
    return obj;
}

Object *__objectInitULong(unsigned long data)
{
    Object *obj = __objectInit(sizeof(unsigned long), ULong);
    *(unsigned long *)(obj->Data) = data;
    return obj;
}

Object *__objectInitULongPointer(unsigned long *data)
{
    size_t typeSize = sizeof(unsigned long);
    if (!__objectCheckPointer(data))
        return NULL;

    Object *obj = __objectInit(typeSize, ULong);
    memcpy(obj->Data, data, typeSize);
    return obj;
}

Object *__objectInitChar(char data)
{
    Object *obj = __objectInit(sizeof(char) * 2, String);
    *(char *)(obj->Data) = data;
    *(char *)(obj->Data + 1) = '\0';
    return obj;
}

Object *__objectInitCharPointer(char *data)
{
    if (!__objectCheckPointer(data))
        return NULL;

    size_t stringSize = strlen(data) + 1;
    if (stringSize < 1)
    {
        __objectReportError(-2);
        return NULL;
    }

    Object *obj = __objectInit(stringSize, String);
    memcpy(obj->Data, data, stringSize);
    return obj;
}

Object *__objectInitPointer(void *data)
{
    size_t typeSize = sizeof(void *);
    Object *obj = __objectInit(typeSize, Pointer);
    obj->Data = data;
    return obj;
}