#ifndef OBJECT_H
#define OBJECT_H

#include <stdint.h>
#include <stdbool.h>
#include <stdarg.h>
#include "../stdio.h"
#include "../stdlib.h"
#include "../stdlib.h"

typedef enum
{
  String,
  Int,
  UInt,
  Double,
  Float,
  Long,
  ULong,
  Pointer,
  None,
} Type;

typedef struct
{
  void *Data;
  size_t Size;
  Type Type;
} Object;

void __objectReportError(const int errorCode);
//Init
Object *__objectInit(size_t typeSize, Type type);
Object *__objectInitFloat(float data);
Object *__objectInitFloatPointer(float *data);
Object *__objectInitDouble(double data);
Object *__objectInitDoublePointer(double *data);
Object *__objectInitInt(int data);
Object *__objectInitIntPointer(int *data);
Object *__objectInitUInt(unsigned int data);
Object *__objectInitUIntPointer(unsigned int *data);
Object *__objectInitLong(long data);
Object *__objectInitLongPointer(long *data);
Object *__objectInitULong(unsigned long data);
Object *__objectInitULongPointer(unsigned long *data);
Object *__objectInitChar(char data);
Object *__objectInitCharPointer(char *data);
Object *__objectInitPointer(void *data);
//Set
bool __objectSetFloat(Object *object, float data);
bool __objectSetFloatPointer(Object *object, float *data);
bool __objectSetDouble(Object *object, double data);
bool __objectSetDoublePointer(Object *object, double *data);
bool __objectSetInt(Object *object, int data);
bool __objectSetIntPointer(Object *object, int *data);
bool __objectSetUInt(Object *object, unsigned int data);
bool __objectSetUIntPointer(Object *object, unsigned int *data);
bool __objectSetLong(Object *object, long data);
bool __objectSetLongPointer(Object *object, long *data);
bool __objectSetULong(Object *object, unsigned long data);
bool __objectSetULongPointer(Object *object, unsigned long *data);
bool __objectSetChar(Object *object, char data);
bool __objectSetCharPointer(Object *object, char *data);
bool __objectSetPointer(Object *object, void *data);
//
bool __objectCheckPointer(void *data);
void ObjectPrint(Object *object);
bool ObjectDelete(Object *object);
/*
    Initialize new data object, with specified value
*/
#define ObjectInit(VALUE) _Generic((VALUE),                     \
                                   float                        \
                                   : __objectInitFloat,         \
                                     float *                    \
                                   : __objectInitFloatPointer,  \
                                     double                     \
                                   : __objectInitDouble,        \
                                     double *                   \
                                   : __objectInitDoublePointer, \
                                     int                        \
                                   : __objectInitInt,           \
                                     int *                      \
                                   : __objectInitIntPointer,    \
                                     unsigned int               \
                                   : __objectInitUInt,          \
                                     unsigned int *             \
                                   : __objectInitUIntPointer,   \
                                     long                       \
                                   : __objectInitLong,          \
                                     long *                     \
                                   : __objectInitLongPointer,   \
                                     unsigned long              \
                                   : __objectInitULong,         \
                                     unsigned long *            \
                                   : __objectInitULongPointer,  \
                                     char                       \
                                   : __objectInitChar,          \
                                     char *                     \
                                   : __objectInitCharPointer,   \
                                     void *                     \
                                   : __objectInitPointer)(VALUE)

#define ObjectSet(OBJECT, VALUE) _Generic((OBJECT, VALUE),            \
                                          float                       \
                                          : __objectSetFloat,         \
                                            float *                   \
                                          : __objectSetFloatPointer,  \
                                            double                    \
                                          : __objectSetDouble,        \
                                            double *                  \
                                          : __objectSetDoublePointer, \
                                            int                       \
                                          : __objectSetInt,           \
                                            int *                     \
                                          : __objectSetIntPointer,    \
                                            unsigned int              \
                                          : __objectSetUInt,          \
                                            unsigned int *            \
                                          : __objectSetUIntPointer,   \
                                            long                      \
                                          : __objectSetLong,          \
                                            long *                    \
                                          : __objectSetLongPointer,   \
                                            unsigned long             \
                                          : __objectSetULong,         \
                                            unsigned long *           \
                                          : __objectSetULongPointer,  \
                                            char                      \
                                          : __objectSetChar,          \
                                            char *                    \
                                          : __objectSetCharPointer,   \
                                            void *                    \
                                          : __objectSetPointer)(OBJECT, VALUE)
#endif //OBJECT_H