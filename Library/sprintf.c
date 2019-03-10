#include <stdarg.h>
#include "stdlib.h"
#include "string.h"
#include "math.h"

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

double _log_base(double n, int base)
{
    return log10(n) / log10(base);
}

int _max(int a, int b)
{
    return a > b ? a : b;
}

unsigned int _number_len(int n, int base)
{
    unsigned int res;

    switch (base)
    {
    case 10:
        res = floor(log10(fabs(n))) + 1;
        break;
    case 16:
    case 8:
        res = floor(_log_base(n, base)) + 1;
        break;
    }

    return res;
}

void _put_integer(char *str, int *put_idx, int number, unsigned short flags, int base, int width, int precision)
{
    size_t int_len = _number_len(number, base); // length of the number
    size_t actual_width = _max(int_len, width); // length of entire string
    //size_t number_len = _max(precision, int_len); // length of number with leading zeros

    bool negative = false;
    if (number < 0)
    {
        number *= -1;
        negative = true;
    }

    char *number_buf = (char *)malloc((int_len + 1) * sizeof(char));
    itoa(number, number_buf, base);

    // 0x and 0
    if (flags & FLAGS_HASH)
    {
        if (base == 8)
        {
            int_len += 1;
        }
        else if (base == 16)
        {
            int_len += 2;
        }
    }

    if (flags & FLAGS_PLUS || flags & FLAGS_SPACE || negative)
    {
        int_len += 1;
    }

    size_t zeros_count = actual_width - _max(precision, int_len);
    size_t space_count = actual_width - int_len - zeros_count;

    // Pre padding
    char prepadding_character = flags & FLAGS_ZEROPAD ? '0' : ' ';
    if (!(flags & FLAGS_LEFT))
    {
        for (int i = space_count; i > 0; i--)
        {
            str[(*put_idx)++] = prepadding_character;
        }
    }

    // Sign
    if (negative)
    {
        str[(*put_idx)++] = '-';
    }
    else if (flags & FLAGS_PLUS)
    {
        str[(*put_idx)++] = '+';
    }
    else if (flags & FLAGS_SPACE)
    {
        str[(*put_idx)++] = ' ';
    }

    // 0x and 0
    if (flags & FLAGS_HASH)
    {
        if (base == 8)
        {
            str[(*put_idx)++] = '0';
        }
        else if (base == 16)
        {
            str[(*put_idx)++] = '0';
            str[(*put_idx)++] = 'x';
        }
    }

    // Zeros
    for (int i = zeros_count; i > 0; i--)
    {
        str[(*put_idx)++] = '0';
    }

    // Actual number
    for (int i = 0; number_buf[i] != '\0'; i++)
    {
        str[(*put_idx)++] = number_buf[i];
    }

    // Post padding
    if (flags & FLAGS_LEFT)
    {
        for (int i = space_count; i > 0; i--)
        {
            str[(*put_idx)++] = ' ';
        }
    }

    free(number_buf);
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

    char *buffer;
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

                _put_integer(str, &put_index, int_arg, flags, 10, width_field, precision_field);
                break;

            case 'o': // OCTAL INTEGER
            {
                int_arg = va_arg(arg, unsigned int);
                _put_integer(str, &put_index, int_arg, flags, 8, width_field, precision_field);
                break;
            }

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
            {
                int_arg = va_arg(arg, unsigned int);
                _put_integer(str, &put_index, int_arg, flags, 16, width_field, precision_field);
                break;
            }

            case '%':
                str[put_index++] = '%';
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