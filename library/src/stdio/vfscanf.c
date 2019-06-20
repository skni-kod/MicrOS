#include <stdarg.h>
#include "../stdio.h"

#define FLAGS_MINUS (1u << 1u)
#define FLAGS_PLUS (1u << 2u)
#define FLAGS_ASTERISK (1u << 3u)

int vfscanf(FILE *stream, const char *format, va_list arg)
{

    printf("SCAN FUNCTION INCOMPLETE, MAY CAUSE UNDEFINED BEHAVIOR!\n");

    // A format specifier for scanf follows this prototype:
    // %[*][width][length]specifier

    for (char *traverse = format; traverse != '\0'; ++traverse)
    {
        if (*traverse != '%')
        {
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
            if (*traverse == '\0')
            {
                // End of string, break the loop
                break;
            }

            switch (*traverse)
            {
            case 'i':
            {
                // @INCOMPLETE
                // Any number of digits, optionally preceded by a sign (+ or -).
                // Decimal digits assumed by default (0-9), but a 0 prefix introduces octal digits (0-7), and 0x hexadecimal digits (0-f).
                // Signed argument.
            }
            break;

            case 'd':
                // @INCOMPLETE
                // Any number of decimal digits (0-9), optionally preceded by a sign. Signed.
                break;

            case 'u':
                // @INCOMPLETE
                // Any number of decimal digits (0-9), optionally preceded by a sign. Unsigned.

                break;

            case 'o':
                // @INCOMPLETE
                // Any number of octal digits (0-7), optionally preceded by a sign (+ or -).
                // Unsigned argument.
                break;

            case 'x':
                // @INCOMPLETE
                // Any number of hexadecimal digits (0-9, a-f, A-F), optionally preceded by 0x or 0X, and all optionally preceded by a sign (+ or -).
                // Unsigned argument.
                break;

            case 'f':
                // @INCOMPLETE
                // A series of decimal digits, optionally containing a decimal point, optionally preceeded by a sign (+ or -)
                // and optionally followed by the e or E character and a decimal integer (or some of the other sequences supported by strtod).
                break;

            case 'e':
                // @INCOMPLETE
                // The same as 'f'
                break;

            case 'g':
                // @INCOMPLETE
                // The same as 'f'
                break;

            case 'a':
                // ???
                // @INCOMPLETE
                break;

            case 'c':
            {
                // The next character. If a width other than 1 is specified, the function reads exactly width characters and stores them
                // in the successive locations of the array passed as argument. No null character is appended at the end.
                char *ch_arg = va_arg(arg, char *);
                *ch_arg = getc(stream);
            }
            break;

            case 's':
                // @INCOMPLETE

                // Any number of non-whitespace characters, stopping at the first whitespace character found.
                // A terminating null character is automatically added at the end of the stored sequence.
                break;

            case 'p':
                // @INCOMPLETE

                //A sequence of characters representing a pointer. The particular format used depends on the system and library implementation,
                //but it is the same as the one used to format %p in fprintf.
                break;

            case 'n':
                // @INCOMPLETE

                // No input is consumed.
                // The number of characters read so far from stdin is stored in the pointed location.
                break;

            case '%':
                // @INCOMPLETE
                // A % followed by another % matches a single %.

                break;

            default:
                break;
            }
        }
    }
}