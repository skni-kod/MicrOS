#include "../vector.h"

/*
    Prints information about error then, exits the program 
    Error codes:
     0  - No error
    -1  - Memory allocation failed
    -2  - Invalid type operation 
*/
void ReportError(unsigned int errorCode)
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
        printf("Empty pointer\n");
        break;
    default:
        break;
    }
}