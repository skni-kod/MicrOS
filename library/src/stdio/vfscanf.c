#include <stdarg.h>
#include "../stdio.h"

int vfscanf(FILE *stream, const char *format, va_list arg)
{

    printf("SCAN FUNCTION INCOMPLETE, MAY CAUSE UNDEFINED BEHAVIOR!\n");

    // A format specifier for scanf follows this prototype:
    // %[*][width][length]specifier

    for (char* traverse = format; traverse != '\0'; ++traverse)
    {
        if(*traverse != '%'){
            // Whitespace character: the function will read and ignore any whitespace
            // characters encountered before the next non-whitespace character 
            // (whitespace characters include spaces, newline and tab characters -- see isspace). 
            // A single whitespace in the format string validates any quantity of whitespace characters 
            // extracted from the stream (including none).

            // Non-whitespace character, except format specifier (%): Any character that is not 
            // either a whitespace character (blank, newline or tab) or part of a format specifier 
            // (which begin with a % character) causes the function to read the next character from the stream, 
            // compare it to this non-whitespace character and if it matches, it is discarded and the function 
            // continues with the next character of format. If the character does not match, the function fails, 
            // returning and leaving subsequent characters of the stream unread.
        }
        else 
        {
            ++traverse;
            if(*traverse == '\0')
            {
                // End of string, break the loop
                break;
            }

            switch (*traverse)
            {
            case 'c':
                {
                    char* ch_arg = va_arg(arg, char*);
                    *ch_arg = getc(stream);
                }
                break;
            
            default:
                break;
            }
        }
    }
    
}