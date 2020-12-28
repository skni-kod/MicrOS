#include "../object.h"

/*
    Prints information about error then, exits the program 
    Error codes:
    -0  - No error
    -1  - Memory allocation failed
    -2  - Invalid type operation
    -3  - Null pointer 
    -4  - Undefined function
    -5  - Invalid type casting
    -6  - Invalid value passed
    -7  - Value overflow
    -8  - Negative to unsigned cast 
*/
void __objectReportError(int errorCode)
{
    switch (errorCode)
    {
    case -1:
        printf("Mermory allocation failed\n");
        exit(-1);
        break;
    case -2:
        printf("Invalid type operation\n");
        break;
    case -3:
        printf("Null pointer\n");
        break;
    case -4:
        printf("Undefined function\n");
        break;
    case -5:
        printf("Invalid type casting\n");
        break;
    case -6:
        printf("Invalid value passed\n");
        break;
    case -7:
        printf("Value overflow\n");
        break;
    case -8:
        printf("Negative to unsigned cast \n");
        break;
    default:
        break;
    }
}