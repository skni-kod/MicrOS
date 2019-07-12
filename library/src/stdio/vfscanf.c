#include <stdarg.h>
#include "../stdio.h"
#include "../ctype.h"

int _get_number_from_file(FILE *stream)
{
    int ret = 0;
    short base = 10;
    bool minus = false;

    char c = getc(stream);

    if (c == '-')
    {
        minus = true;
        c = getc(stream);
    }

    if (c == '0')
    {
        // number is octal
        base = 8;

        c = getc(stream);
        if (c == 'x' || c == 'X')
        {
            // number is hexadecimal
            base = 16;
            c = getc(stream);
        }
    }

    while (!isspace(c))
    {
        int digit;
        if (c >= '0' && c <= '9')
            digit = c - '0';
        else if (c >= 'A' && c <= 'F')
            digit = c - 'A' + 10;
        else if (c >= 'a' && c <= 'f')
            digit = c - 'a' + 10;

        ret = ret * base + digit;
        c = getc(stream);
    }

    if (minus)
        ret *= -1;

    return ret;
}

int _get_decimal_number_from_file(FILE *stream)
{
    int ret = 0;
    bool minus = false;

    char c = getc(stream);

    if (c == '-')
    {
        minus = true;
        c = getc(stream);
    }

    while (!isspace(c))
    {
        int digit = c - '0';

        ret = ret * 10 + digit;
        c = getc(stream);
    }

    if (minus)
        ret *= -1;

    return ret;
}

int _get_hex_number_from_file(FILE *stream)
{
    int ret = 0;
    short base = 16;
    bool minus = false;

    char c = getc(stream);

    if (c == '-')
    {
        minus = true;
        c = getc(stream);
    }

    if (c == '0')
    {
        c = getc(stream);
        if (c == 'x' || c == 'X')
        {
            // number is hexadecimal
            base = 16;
            c = getc(stream);
        }
    }

    while (!isspace(c))
    {
        int digit;
        if (c >= '0' && c <= '9')
            digit = c - '0';
        else if (c >= 'A' && c <= 'F')
            digit = c - 'A' + 10;
        else if (c >= 'a' && c <= 'f')
            digit = c - 'a' + 10;

        ret = ret * base + digit;
        c = getc(stream);
    }

    if (minus)
        ret *= -1;

    return ret;
}

int _get_octal_number_from_file(FILE *stream)
{
    int ret = 0;
    short base = 8;
    bool minus = false;

    char c = getc(stream);

    if (c == '-')
    {
        minus = true;
        c = getc(stream);
    }

    if (c == '0')
    {
        // skip this character
        c = getc(stream);
    }

    while (!isspace(c))
    {
        int digit = c - '0';

        ret = ret * base + digit;
        c = getc(stream);
    }

    if (minus)
        ret *= -1;

    return ret;
}

int vfscanf(FILE *stream, const char *format, va_list arg)
{

    printf("SCAN FUNCTION INCOMPLETE, MAY CAUSE UNDEFINED BEHAVIOR!\n");

    unsigned short flags = 0;
    int width_field = 0;
    bool have_asterisk = false;

    // A format specifier for scanf follows this prototype:
    // %[*][width][length]specifier

    for (char *traverse = format; *traverse != '\0'; ++traverse)
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

            // Check for asterisk field
            if (*traverse == '*')
            {
                have_asterisk = true;
                ++traverse;
            }

            // Evaluate width field
            // @TODO find way to reset width field
            while (*traverse >= '0' && *traverse <= '9')
            {
                int digit = (int)(*traverse);
                width_field = width_field * 10 + digit;
            }

            // @TODO evaluate length field

            switch (*traverse)
            {
            case 'i':
            {
                // @INCOMPLETE
                // Any number of digits, optionally preceded by a sign (+ or -).
                // Decimal digits assumed by default (0-9), but a 0 prefix introduces octal digits (0-7), and 0x hexadecimal digits (0-f).
                // Signed argument.
                int *int_ptr = va_arg(arg, int *);
                int n = _get_number_from_file(stream);

                *int_ptr = n;
            }
            break;

            case 'd':
            case 'u':
                // @INCOMPLETE
                // Any number of decimal digits (0-9), optionally preceded by a sign. Unsigned.
                {
                    int *int_ptr = va_arg(arg, int *);
                    int n = _get_decimal_number_from_file(stream);

                    *int_ptr = n;
                }
                break;

            case 'o':
                // @INCOMPLETE
                // Any number of octal digits (0-7), optionally preceded by a sign (+ or -).
                // Unsigned argument.
                {
                    int *int_ptr = va_arg(arg, int *);
                    int n = _get_octal_number_from_file(stream);

                    *int_ptr = n;
                }
                break;

            case 'x':
                // @INCOMPLETE
                // Any number of hexadecimal digits (0-9, a-f, A-F), optionally preceded by 0x or 0X, and all optionally preceded by a sign (+ or -).
                // Unsigned argument.
                {
                    int *int_ptr = va_arg(arg, int *);
                    int n = _get_hex_number_from_file(stream);

                    *int_ptr = n;
                }
                break;

            case 'f':
                // @INCOMPLETE, see strtod.c for more detail

                // A series of decimal digits, optionally containing a decimal point, optionally preceeded by a sign (+ or -)
                // and optionally followed by the e or E character and a decimal integer (or some of the other sequences supported by strtod).
                float *float_ptr = va_arg(arg, float *);
                
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
                {
                    char *char_ptr = va_arg(arg, char *);
                    char c = getc(stream);
                    int index = 0;

                    while (!isspace(c))
                    {
                        char_ptr[index++] = c;
                        c = getc(stream);
                    }

                    char_ptr[index] = 0;
                }
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