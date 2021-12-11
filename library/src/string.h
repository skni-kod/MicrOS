#ifndef STRING_H
#define STRING_H

#include "stdint.h"
#include "stddef.h"

//! Unsigned integral type.
typedef size_t uint32_t;

#ifdef __cplusplus
extern "C" {
#endif

//! Copy block of memory.
/*!
    Copies the values of num bytes from the location pointed to by source directly to the memory block pointed to by destination.
    \param destination Pointer to the destination array where the content is to be copied, type-casted to a pointer of type void*.
    \param source Pointer to the source of data to be copied, type-casted to a pointer of type const void*.
    \param num Number of bytes to copy.
    \return Destination is returned.
*/
void *memcpy(void *destination, const void *source, size_t size);

//! Move block of memory.
/*!
    Copies the values of num bytes from the location pointed by source to the memory block pointed by destination. Copying takes place as if an intermediate buffer were used, allowing the destination and source to overlap.
    \param destination Pointer to the destination array where the content is to be copied, type-casted to a pointer of type void*.
    \param source Pointer to the source of data to be copied, type-casted to a pointer of type const void*.
    \param num Number of bytes to copy.
    \return Destination is returned.
*/
void *memmove(void *destination, const void *source, size_t size);

//! Copy string.
/*!
    Copies the C string pointed by source into the array pointed by destination, including the terminating null character (and stopping at that point).
    \param destination Pointer to the destination array where the content is to be copied.
    \param source C string to be copied.
    \return Destination is returned.
*/
char *strcpy(char *destination, const char *source);

//! Copy characters from string.
/*!
    Copies the first num characters of source to destination. If the end of the source C string (which is signaled by a null-character) is found before num characters have been copied, destination is padded with zeros until a total of num characters have been written to it.
    \param destination Pointer to the destination array where the content is to be copied.
    \param source C string to be copied.
    \param num Maximum number of characters to be copied from source.
    \return Destination is returned.
*/
char *strncpy(char *destination, const char *source, size_t num);

//! Concatenate strings.
/*!
    Appends a copy of the source string to the destination string. The terminating null character in destination is overwritten by the first character of source, and a null-character is included at the end of the new string formed by the concatenation of both in destination.
    \param destination Pointer to the destination array, which should contain a C string, and be large enough to contain the concatenated resulting string.
    \param source C string to be appended. This should not overlap destination.
    \return Destination is returned.
*/
char *strcat(char *destination, const char *source);

//! Append characters from string.
/*!
    Appends the first num characters of source to destination, plus a terminating null-character.
    \param destination Pointer to the destination array, which should contain a C string, and be large enough to contain the concatenated resulting string, including the additional null-character.
    \param source C string to be appended.
    \param num Maximum number of characters to be appended.
    \return Destination is returned.
*/
char *strncat(char *destination, const char *source, size_t num);

//! Compare two blocks of memory.
/*!
    Compares the first num bytes of the block of memory pointed by ptr1 to the first num bytes pointed by ptr2, returning zero if they all match or a value different from zero representing which is greater if they do not.
    \param ptr1 Pointer to block of memory.
    \param ptr2 Pointer to block of memory.
    \param num Number of bytes to compare.
    \return Returns an integral value indicating the relationship between the content of the memory blocks (<0: value in ptr1 was lower than in ptr2, 0: both are equals, >0: value in ptr2 was greater than in ptr2).
*/
int memcmp(const void *buffer1, const void *buffer2, size_t size);

//! Compare two strings.
/*!
    Compares the C string str1 to the C string str2. This function starts comparing the first character of each string. If they are equal to each other, it continues with the following pairs until the characters differ or until a terminating null-character is reached.
    \param str1 C string to be compared.
    \param str2 C string to be compared.
    \return Returns an integral value indicating the relationship between the content of the memory blocks (<0: value in ptr1 was lower than in ptr2, 0: both are equals, >0: value in ptr2 was greater than in ptr2).
*/
int strcmp(const char *str1, const char *str2);

//! Compare two strings using locale.
/*!
    Compares the C string str1 to the C string str2, both interpreted appropriately according to the LC_COLLATE category of the C locale currently selected.
    \param str1 C string to be compared.
    \param str2 C string to be compared.
    \param num Maximum number of characters to compare.
    \return Returns an integral value indicating the relationship between the content of the memory blocks (<0: value in ptr1 was lower than in ptr2, 0: both are equals, >0: value in ptr2 was greater than in ptr2).
*/
int strcoll(const char *str1, const char *str2);

//! Compare two strings using locale.
/*!
    Compares up to num characters of the C string str1 to those of the C string str2.
    \param str1 C string to be compared.
    \param str2 C string to be compared.
    \return Returns an integral value indicating the relationship between the content of the memory blocks (<0: value in ptr1 was lower than in ptr2, 0: both are equals, >0: value in ptr2 was greater than in ptr2).
*/
int strncmp(const char *str1, const char *str2, size_t num);

//! Transform string using locale.
/*!
    Transforms the C string pointed by source according to the current locale and copies the first num characters of the transformed string to destination, returning its length.
    \param destinatino Pointer to the destination array where the content is to be copied. It can be a null pointer if the argument for num is zero.
    \param source C string to be transformed.
    \param num Maximum number of characters to be copied to destination.
    \return The length of the transformed string, not including the terminating null-character.
*/
size_t strxfrm(char *destination, const char *source, size_t num);

//! Locate character in block of memory.
/*!
    Searches within the first num bytes of the block of memory pointed by ptr for the first occurrence of value (interpreted as an unsigned char), and returns a pointer to it.
    \param ptr Pointer to the block of memory where the search is performed.
    \param value Value to be located. The value is passed as an int, but the function performs a byte per byte search using the unsigned char conversion of this value.
    \param num Number of bytes to be analyzed.
    \return A pointer to the first occurrence of value in the block of memory pointed by ptr. If the value is not found, the function returns a null pointer.
*/
void *memchr(void *ptr, int value, size_t num);

//! Locate first occurrence of character in string.
/*!
    Returns a pointer to the first occurrence of character in the C string str.
    \param str C string.
    \param character Character to be located. It is passed as its int promotion, but it is internally converted back to char for the comparison.
    \return A pointer to the first occurrence of value in the block of memory pointed by ptr. If the value is not found, the function returns a null pointer.
*/
char *strchr(const char *str, int character);

//! Get span until character in string.
/*!
    Scans str1 for the first occurrence of any of the characters that are part of str2, returning the number of characters of str1 read before this first occurrence.
    \param str1 C string to be scanned.
    \param str2 C string containing the characters to match.
    \return The length of the initial part of str1 not containing any of the characters that are part of str2. This is the length of str1 if none of the characters in str2 are found in str1.
*/
size_t strcspn(const char *str1, const char *str2);

//! Locate characters in string.
/*!
    Returns a pointer to the first occurrence in str1 of any of the characters that are part of str2, or a null pointer if there are no matches.
    \param str1 C string to be scanned.
    \param str2 C string containing the characters to match.
    \return A pointer to the first occurrence in str1 of any of the characters that are part of str2, or a null pointer if none of the characters of str2 is found in str1 before the terminating null-character.
*/
char *strpbrk(char *str1, const char *str2);

//! Locate last occurrence of character in string.
/*!
    Returns a pointer to the last occurrence of character in the C string str.
    \param str1 C string.
    \param character Character to be located. It is passed as its int promotion, but it is internally converted back to char.
    \return A pointer to the last occurrence of character in str. If the character is not found, the function returns a null pointer.
*/
char *strrchr(char *str, int character);

//! Get span of character set in string.
/*!
    Returns the length of the initial portion of str1 which consists only of characters that are part of str2.
    \param str1 C string to be scanned.
    \param str2 C string containing the characters to match.
    \return The length of the initial portion of str1 containing only characters that appear in str2.
*/
size_t strspn(const char *str1, const char *str2);

//! Locate substring.
/*!
    Returns a pointer to the first occurrence of str2 in str1, or a null pointer if str2 is not part of str1.
    \param str1 C string to be scanned.
    \param str2 C string containing the characters to match.
    \return A pointer to the first occurrence in str1 of the entire sequence of characters specified in str2, or a null pointer if the sequence is not present in str1.
*/
char *strstr(const char *str1, const char *str2);

//! Split string into tokens.
/*!
    A sequence of calls to this function split str into tokens, which are sequences of contiguous characters separated by any of the characters that are part of delimiters.
    \param str C string to truncate.
    \param delimeters C string containing the delimiter characters.
    \return If a token is found, a pointer to the beginning of the token. Otherwise, a null pointer.
*/
char *strtok(char *str, const char *delimiters);

//! Fill block of memory.
/*!
    Sets the first num bytes of the block of memory pointed by ptr to the specified value (interpreted as an unsigned char).
    \param buffer Pointer to the block of memory to fill.
    \param value Value to be set. The value is passed as an int, but the function fills the block of memory using the unsigned char conversion of this value.
    \param size Number of bytes to be set to the value.
    \return Ptr is returned.
*/
void *memset(void *buffer, int value, size_t size);

//! Get pointer to error message string.
/*!
    Interprets the value of errnum, generating a string with a message that describes the error condition as if set to errno by a function of the library.
    \param errnum Error number.
    \return A pointer to the error string describing error errnum.
*/
char *strerror(int errnum);

//! Get string length.
/*!
    Returns the length of the C string str. The length of a C string is determined by the terminating null-character.
    \param str C string.
    \return The length of string. 
*/
size_t strlen(const char *str);

#ifdef __cplusplus
}
#endif

#endif