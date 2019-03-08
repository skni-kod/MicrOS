#include <stdarg.h>
#include "stdlib.h"

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

int sscanf(const char *str, const char *format, ...)
{
    return 0;
}

bool _is_digit(char c)
{
    return c >= '0' && c <= '9';
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
                int_arg = va_arg(arg, int);
                str[put_index++] = int_arg;
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

                // Put 0 as indicator that is octal number
                str[put_index++] = '0';

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
                // put arg to buffer
                index = 0;
                while (str_arg[index] != '\0')
                {
                    str[put_index++] = str_arg[index];
                    index++;
                }
                break;

            case 'x': // HEX INTEGER
                int_arg = va_arg(arg, unsigned int);
                // convert arg to string with 16 as base
                itoa(int_arg, buffer, 16);

                // put 0x as this is hex number
                str[put_index++] = '0';
                str[put_index++] = 'x';

                index = 0;
                while (buffer[index] != '\0' && index < BUFFER_SIZE)
                {
                    str[put_index++] = buffer[index];
                    index++;
                }

                break;
            }
        }
    }

    str[put_index++] = '\0';

    va_end(arg);
    return 0;
}