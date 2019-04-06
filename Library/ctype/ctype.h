#ifndef CTYPE_H
#define CTYPE_H

#ifdef __cplusplus
extern "C" {
#endif

//! Check if value is alfanumeric.
/*!
    The isalnum function tests for any character for which isalpha or isdigit is true.
    \param c Value to check.
    \return 1 if true, 0 if false
*/
int isalnum(int c);

//! Check if value is letter.
/*! 
    The isalpha function tests for any character for which isupper or islower is true, or any character that is one of a locale-specific set of alphabetic characters for which none of iscntrl, isdigit, ispunct, or isspace is true. In the "C" locale, isalpha returns true only for the characters for which isupper or islower is true.
    \param c Value to check.
    \return 1 if true, 0 if false
*/
int isalpha(int c);

//! Check if value is blank.
/*!  
    The isblank function tests for any character that is a standard blank character or is one of a locale-specific set of characters for which isspace is true and that is used to separate words within a line of text. The standard blank characters are the following: space (' '), and horizontal tab ('\t'). In the "C" locale, isblank returns true only for the standard blank characters.
    \param c Value to check.
    \return 1 if true, 0 if false
*/
int isblank(int c);

//! Check if value is control character.
/*! 
    The iscntrl function tests for any control character.
    \param c Value to check.
    \return 1 if true, 0 if false
*/
int iscntrl(int c);

//! Check if value is digit.
/*! 
    The isdigit function tests for any decimal-digit character.
    \param c Value to check.
    \return 1 if true, 0 if false
*/
int isdigit(int c);

//! Check if value is any printing character except space.
/*! 
    The isgraph function tests for any printing character except space (' ').
    \param c Value to check.
    \return 1 if true, 0 if false
*/
int isgraph(int c);

//! Check if value is lower alphanumeric.
/*!  
    The islower function tests for any character that is a lowercase letter or is one of a locale-specific set of characters for which none of iscntrl, isdigit, ispunct, or isspace is true. In the "C" locale, islower returns true only for the lowercase letters.
    \param c Value to check.
    \return 1 if true, 0 if false
*/
int islower(int c);

//! Check if value is printable.
/*!  
    The isprint function tests for any printing character including space (' ').
    \param c Value to check.
    \return 1 if true, 0 if false
*/
int isprint(int c);

//! Check if value is punctation.
/*!  
    The ispunct function tests for any printing character that is one of a locale-specific set of punctuation characters for which neither isspace nor isalnum is true. In the "C" locale, ispunct returns true for every printing character for which neither isspace nor isalnum is true.
    \param c Value to check.
    \return 1 if true, 0 if false
*/
int ispunct(int c);

//! Check if value is space.
/*! 
    The isspace function tests for any character that is a standard white-space character or is one of a locale-specific set of characters for which isalnum is false. The standard white-space characters are the following: space (' '), form feed ('\f'), new-line ('\n'), carriage return ('\r'), horizontal tab ('\t'), and vertical tab ('\v'). In the "C" locale, isspace returns true only for the standard white-space characters.
    \param c Value to check.
    \return 1 if true, 0 if false
*/
int isspace(int c);

//! Check if value is upper alphanumeric
/*!  
    The isupper function tests for any character that is an uppercase letter or is one of a locale-specific set of characters for which none of iscntrl, isdigit, ispunct, or isspace is true. In the "C" locale, isupper returns true only for the uppercase letters.
    \param c Value to check.
    \return 1 if true, 0 if false
*/
int isupper(int c);

//! Check if value is hexadecimal-digit character.
/*! 
    The isxdigit function tests for any hexadecimal-digit character.
    \param c Value to check.
    \return 1 if true, 0 if false
*/
int isxdigit(int c);

//! Make letter lower.
/*! 
    The tolower function converts an uppercase letter to a corresponding lowercase letter.
    \param c Value to check.
    \return If the argument is a character for which isupper is true and there are one or more corresponding characters, as specified by the current locale, for which islower is true, the tolower function returns one of the corresponding characters (always the same one for any giv en locale); otherwise, the argument is returned unchanged.
*/
int tolower(int c);

//! Make letter upper.
/*! 
    The toupper function converts a lowercase letter to a corresponding uppercase letter.
    \param c Value to check.
    \return If the argument is a character for which islower is true and there are one or more corresponding characters, as specified by the current locale, for which isupper is true, the toupper function returns one of the corresponding characters (always the same one for any giv en locale); otherwise, the argument is returned unchanged.
*/
int toupper(int c);

#ifdef __cplusplus
}
#endif

#endif //CTYPE_H