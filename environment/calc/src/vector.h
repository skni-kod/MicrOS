#define _CRT_SECURE_NO_WARNINGS
#define _USE_MATH_DEFINES
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#ifndef ARRAY_H
#define ARRAY_H

typedef enum type_
{
	String,
	Int,
	Double,
	Pointer,
} Type;

typedef struct object
{
	void *Data;
	size_t Size;
	Type Type;
} Object;

typedef struct vector
{
	void **Data;
	size_t Size;
} Vector;

/* Vector */
Vector *VectorInit(void);
void VectorAppend(Vector *vector, const Object *object, const bool copy);
Object *VectorPop(Vector *vector);
void VectorDelete(Vector *vector, const int index);
void VectorSet(Vector *vector, const Object *object, const unsigned int index);
void VectorInsert(Vector *vector, const Object *object, const unsigned int index);
void VectorPrint(Vector *vector);
void VectorDestroy(Vector *vector);

/* Data object */
Object *ObjectInit(const void *data, const Type type);
size_t ObjectDataSize(const void *data, const Type type);
void ObjectSet(Object *object, const void *data, const Type type);
void ObjectPrint(Object *object);
void ObjectDelete(Object *object);
void ObjectToString(Object *object);
void ObjectToInt(Object *object);
void ObjectToDouble(Object *object);

/* String */
void StringAppend(Object *object, const char *string);
void StringCut(Object *object, const unsigned int index, const int count);
void StringSplit(Vector *vector, Object *object, const char *delimiter);
void StringTrim(Object *object, bool direction, const char data);
void StringReverse(Object *object);
unsigned int StringLength(Object *object);
bool isDigit(const char data);
bool isNumber(const char *data);

/* Utility */
void ReportError(unsigned int errorCode);

#endif