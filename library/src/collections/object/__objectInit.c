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

bool __objectCheckData(void *data)
{
    if (data != NULL)
        return true;
    else
    {
        __objectReportError(-3);
        return false;
    }
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
    if (!__objectCheckData(data))
        return NULL;

    Object *obj = __objectInit(typeSize, Int);
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
    if (!__objectCheckData(data))
        return NULL;

    Object *obj = __objectInit(typeSize, Double);
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
    if (!__objectCheckData(data))
        return NULL;

    size_t stringSize = strlen(data);
    if (stringSize == 0)
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