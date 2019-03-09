#include <stdarg.h>
#include "stdlib.h"
#include "string.h"
#include "math.h"

#define BUFFER_SIZE 16

#define FLAGS_ZEROPAD (1U << 0U)
#define FLAGS_LEFT (1U << 1U)
#define FLAGS_PLUS (1U << 2U)
#define FLAGS_SPACE (1U << 3U)
#define FLAGS_HASH (1U << 4U)
#define FLAGS_UPPERCASE (1U << 5U)
#define FLAGS_CHAR (1U << 6U)
#define FLAGS_SHORT (1U << 7U)
#define FLAGS_LONG (1U << 8U)
#define FLAGS_LONG_LONG (1U << 9U)
#define FLAGS_PRECISION (1U << 10U)

bool _is_digit(char c)
{
    return c >= '0' && c <= '9';
}

unsigned int _number_len(int n)
{
    unsigned int res = floor(log10(abs(n))) + 1;
    if (n < 0)
        res++;

    return res;
}

unsigned int _parse_number_field(const char **str)
{
    unsigned int ret = 0;
    while (_is_digit(**str))
    {
        ret = ret * 10 + (unsigned int)(**str - '0');
        (*str)++;
    }

    return ret;
}

int sprintf(char *str, const char *format, ...)
{
    va_list arg;
    va_start(arg, format);

    int int_arg;
    char *str_arg;
    unsigned int put_index = 0;

    char buffer[BUFFER_SIZE];
    int index = 0;

    unsigned short flags = 0;
    int width_field = 0;
    int precision_field = 0;

    char *traverse;
    for (traverse = format; *traverse != '\0'; ++traverse)
    {

        // if travese is '%' just put character to buffer
        // else go to next character, check if it's not null
        // and then parse arguments

        if (*traverse != '%')
        {
            str[put_index++] = *traverse;
        }
        else
        {
            // Check next character
            ++traverse;
            if (*traverse == '\0')
            {
                // End of string, break loop
                break;
            }

            // Parse format string
            // %[flags][width][.precision][length]specifier

            // evaluate flags
            bool f = true;
            do
            {
                switch (*traverse)
                {
                case '0':
                    flags |= FLAGS_ZEROPAD;
                    ++traverse;
                    break;
                case '-':
                    flags |= FLAGS_LEFT;
                    ++traverse;
                    break;
                case '+':
                    flags |= FLAGS_PLUS;
                    ++traverse;
                    break;
                case ' ':
                    flags |= FLAGS_SPACE;
                    ++traverse;
                    break;
                case '#':
                    flags |= FLAGS_HASH;
                    ++traverse;
                    break;
                default:
                    f = false;
                    break;
                }
            } while (f);

            // evaluate width
            if (_is_digit(*traverse))
            {
                width_field = _parse_number_field(&traverse);
            }
            else if (*traverse == '*')
            {
                int w = va_arg(arg, int);
                if (w < 0)
                {
                    flags |= FLAGS_LEFT;
                    width_field = -w;
                }
                else
                {
                    width_field = w;
                }
                ++traverse;
            }

            // evaluate precision
            if (*traverse == '.')
            {
                flags |= FLAGS_PRECISION;
                ++traverse;
                if (_is_digit(*traverse))
                {
                    precision_field = _parse_number_field(&traverse);
                }
                else if (*traverse == '*')
                {
                    int prec = va_arg(arg, int);
                    precision_field = prec > 0 ? prec : 0;
                    ++traverse;
                }
            }

            // evaluate length field
            switch (*traverse)
            {
            case 'l':
                flags |= FLAGS_LONG;
                ++traverse;
                if (*traverse == 'l')
                {
                    flags |= FLAGS_LONG_LONG;
                    ++traverse;
                }
                break;
            case 'h':
                flags |= FLAGS_SHORT;
                ++traverse;
                if (*traverse == 'h')
                {
                    flags |= FLAGS_CHAR;
                    ++traverse;
                }
                break;

            case 't':
                flags |= (sizeof(ptrdiff_t) == sizeof(long) ? FLAGS_LONG : FLAGS_LONG_LONG);
                ++traverse;
                break;

            case 'j':
                flags |= (sizeof(intmax_t) == sizeof(long) ? FLAGS_LONG : FLAGS_LONG_LONG);
                ++traverse;
                break;

            case 'z':
                flags |= (sizeof(size_t) == sizeof(long) ? FLAGS_LONG : FLAGS_LONG_LONG);
                ++traverse;
                break;
            }

            // Parse arguments
            switch (*traverse)
            {
            case 'c': // CHARACTER
                int_arg = va_arg(arg, unsigned int);

                // Right padding 
                if (!(flags & FLAGS_LEFT))
                {
                    for (int i = width_field - 1; i > 0; i--)
                    {
                        str[put_index++] = ' ';
                    }
                }

                // Put character
                str[put_index++] = int_arg;

                // Left padding
                if (flags & FLAGS_LEFT)
                {
                    for (int i = width_field - 1; i > 0; i--)
                    {
                        str[put_index++] = ' ';
                    }
                }
                break;

            case 'd': // INTEGER
            case 'i':
                int_arg = va_arg(arg, int);

                // convert argument to string and put to buffer
                itoa(int_arg, buffer, 10);
                index = 0;
                while (buffer[index] != '\0' && index < BUFFER_SIZE)
                {
                    str[put_index++] = buffer[index];
                    index++;
                }
                break;

            case 'o': // OCTAL INTEGER
                int_arg = va_arg(arg, unsigned int);
                // convert argumnrnt to string with 8
                itoa(int_arg, buffer, 8);

                if (flags & FLAGS_HASH)
                {
                    // Put 0 as indicator that is octal number
                    str[put_index++] = '0';
                }

                // put argument
                index = 0;
                while (buffer[index] != '\0' && index < BUFFER_SIZE)
                {
                    str[put_index++] = buffer[index];
                    index++;
                }
                break;

            case 's': // STRING
                str_arg = va_arg(arg, char *);
                int str_len = strlen(str_arg);

                // Check maximum number of characters (precision field)
                if (flags & FLAGS_PRECISION)
                {
                    str_len = str_len > precision_field ? precision_field : str_len;
                }

                // Right padding 
                if (!(flags & FLAGS_LEFT))
                {
                    for (int i = (width_field - str_len); i > 0; i--)
                    {
                        str[put_index++] = ' ';
                    }
                }

                // put arg to buffer
                for (int i = 0; i < str_len; i++)
                {
                    str[put_index++] = str_arg[i];
                }

                // Left Padding
                if (flags & FLAGS_LEFT)
                {
                    for (int i = (width_field - str_len); i > 0; i--)
                    {
                        str[put_index++] = ' ';
                    }
                }
                break;

            case 'X':
            case 'x': // HEX INTEGER
                int_arg = va_arg(arg, unsigned int);
                // convert arg to string with 16 as base
                itoa(int_arg, buffer, 16);

                if (flags & FLAGS_HASH)
                {
                    // put 0x as this is hex number
                    str[put_index++] = '0';
                    str[put_index++] = 'x';
                }

                index = 0;
                while (buffer[index] != '\0' && index < BUFFER_SIZE)
                {
                    str[put_index++] = buffer[index];
                    index++;
                }

                break;

            case '%':
                str[put_index++] = '5';
            break;

            default: 
                str[put_index++] = *traverse;
            break;
            }
        }
    }

    str[put_index++] = '\0';

    va_end(arg);
    return 0;
}