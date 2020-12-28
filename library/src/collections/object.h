#ifndef OBJECT_H
#define OBJECT_H

#include <stdint.h>
#include <stdbool.h>
#include <stdarg.h>
#include <limits.h>
#include "../stdio.h"
#include "../stdlib.h"
#include "../string.h"

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

typedef enum
{
  TrimLeading,
  TrimTailing
} TrimMode;

//! Print error
/*!
    Print error information to standard output;
    \param errorCode Error code
    \return Nothing
*/
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
//! Check if pointer valid
/*!
    \param data Pointer  
    \return Returns true when pointer is valid
*/
bool __objectCheckPointer(void *data);
//! Print value stored in object
/*!
    Print value stored in object to standard output;
    \param object Pointer to object  
    \return Nothing
*/
void ObjectPrint(Object *object);
//! Delete object from memory
/*!
    Delete object from memory
    \param object Pointer to object  
    \return When succeeds, returns true
*/
bool ObjectDelete(Object *object);

//! Initialize new object with specified value
/*!
    Get pointer to new object that stores passed value.
    Supported data types: int, unsigned int, float, double, long, unsigned long, char, void*
    \param VALUE Value stored in object. Can be passed both ways.
    \return Pointer to new object.
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

//! Set data to specified object
/*!
    Set new value stored in exisiting object
    Supported data types: int, unsigned int, float, double, long, unsigned long, char, void*
    \param OBJECT Pointer to object it's value set
    \param VALUE Value stored in object. Can be passed both ways.
    \return When succeeds, returns true
*/
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

//! Convert data stored in object to integer
/*!
    Convert data stored in object to integer
    \param object Pointer to object  
    \return When succeeds, returns true
*/
bool ObjectToInt(Object *object);

//! Convert data stored in object to unsigned integer
/*!
    Convert data stored in object to unsigned integer
    \param object Pointer to object  
    \return When succeeds, returns true
*/
bool ObjectToUInt(Object *object);

//! Convert data stored in object to long
/*!
    Convert data stored in object to long
    \param object Pointer to object  
    \return When succeeds, returns true
*/
bool ObjectToLong(Object *object);

//! Convert data stored in object to unsigned long
/*!
    Convert data stored in object to unsigned long
    \param object Pointer to object  
    \return When succeeds, returns true
*/
bool ObjectToULong(Object *object);

//! Convert data stored in object to float
/*!
    Convert data stored in object to float
    \param object Pointer to object  
    \return When succeeds, returns true
*/
bool ObjectToFloat(Object *object);

//! Convert data stored in object to double
/*!
    Convert data stored in object to double
    \param object Pointer to object  
    \return When succeeds, returns true
*/
bool ObjectToDouble(Object *object);

//! Reverse string
/*!
    Reverse string stored in object
    \param OBJECT Pointer to object
    \return Returns true when succeeds
*/
bool StringReverse(Object *object);

//! Convert data stored in object to String type
/*!
     Convert data stored in object to String type
    \param object Pointer to object  
    \return When succeeds, returns true
*/
bool ObjectToString(Object *object);

int __stringAppendChar(Object *object, char c);
int __stringAppendCharPointer(Object *object, char *str);

//! Append value to string
/*!
    Append value to object that stores string
    \param OBJECT Pointer to object
    \param STRING Value append to string
    \return Number of appended characters
*/
#define StringAppend(OBJECT, VALUE) _Generic((OBJECT, VALUE),      \
                                             int                   \
                                             : __stringAppendChar, \
                                               char                \
                                             : __stringAppendChar, \
                                               char *              \
                                             : __stringAppendCharPointer)(OBJECT, VALUE)

//! StringTrim
/*!
    Makes string stored in object in which all leading/trailing occurrences of specified\
    character from the current string are removed.
    \param object Pointer to object
    \param data Removed char
    \param mode Leading/Tailing
    \return Numer of removed characters
*/
int StringTrim(Object *object, const char data, TrimMode mode);

//! StringCut
/*!
    Cut content of string in specified range.
    if count is greater than 0, it cuts specified amount of characters on to the right,
    if count is less than 0, it cuts specified amount of characters on to the left
    of the position in the string specified by index
    \param object Pointer to object
    \param index Specified place in string
    \param amount Amount of removed characters
    \return Numer of removed characters
*/
int StringCut(Object *object, const unsigned int index, const int amount);

//! StringLast
/*!
    Get index of last non zero character in string
    \param object Pointer to object
    \return Index of last non zero character in string
*/
int StringLast(Object *object);

#endif //OBJECT_H