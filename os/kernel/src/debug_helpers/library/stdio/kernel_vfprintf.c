#include <stdarg.h>
#include "stdlib.h"
#include "string.h"
#include "math.h"
#include "stdio.h"
#include "stdbool.h"
#include "stdint.h"
#include "../../../memory/heap/heap.h"

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
#define FLAGS_LONG_DOUBLE (1U << 11U)

typedef long long int i64;
typedef unsigned long long int u64;

// HELPER FUNCTIONS
bool _kernel_is_digit(char c)
{
    return c >= '0' && c <= '9';
}

int _kernel_max(int a, int b)
{
    return a > b ? a : b;
}

unsigned int _kernel_unsigned_number_len(u64 n, int base)
{
    unsigned int res = 1;

    while (n >= (unsigned int)base)
    {
        res++;
        n /= base;
    }

    return res;
}

unsigned int _kernel_fnumber_len(long double n)
{
    if (n == 0)
        return 1;
    return floor(log10(fabs(n))) + 1;
}

char *_kernel_itoa(u64 number, char *buffer, int base, bool uppercase, int size)
{
    int idx = size - 1;
    static const char lowercase_table[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};
    static const char uppercase_table[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};

    do
    {
        int digit = (number % base);
        buffer[idx--] = uppercase ? (uppercase_table[digit]) : (lowercase_table[digit]);

        number /= base;
    } while (number);
    buffer[size] = '\0';

    return buffer;
}

char *_kernel_ftoa(long double number, char *buffer, unsigned short flags, int precision)
{

    float whole = floor(number);
    float frac = number - whole;

    // change frac value so it has no significant zeros
    // they will be printed later
    int num_zeros = 0;
    while (frac < 0.1)
    {
        frac *= 10;
        num_zeros++;
    }

    frac = floor(pow(10, precision - num_zeros) * frac);

    int whole_size = _kernel_fnumber_len(whole);
    int frac_size = precision;

    int idx = whole_size;

    while (idx)
    {
        buffer[idx - 1] = (char)fmod(whole, 10) + '0';
        whole /= 10;
        idx--;
    }

    if (precision != 0 || flags & FLAGS_HASH)
    {
        buffer[whole_size] = '.';
    }

    idx = whole_size + frac_size;
    buffer[idx + 1] = '\0';

    while (frac >= 1)
    {
        buffer[idx--] = (char)fmod(frac, 10) + '0';
        frac /= 10;
    }

    idx = whole_size + 1;
    for (int i = 0; i < num_zeros; i++)
    {
        buffer[idx++] = '0';
    }

    return buffer;
}

void _kernel_put_unsigned_integer(FILE *stream, unsigned int *put_idx, u64 number, unsigned short flags, int base, int width, int precision)
{
    int int_len = _kernel_unsigned_number_len(number, base); // length of the number

    char *number_buf = (char *)heap_kernel_alloc((int_len + 1) * sizeof(char), 0);
    _kernel_itoa(number, number_buf, base, flags & FLAGS_UPPERCASE, int_len);

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

    if (flags & FLAGS_PLUS || flags & FLAGS_SPACE)
    {
        int_len += 1;
    }

    int zeros_count = precision - int_len;
    int space_count = width - _kernel_max(precision, int_len);

    // Pre padding
    char prepadding_character = flags & FLAGS_ZEROPAD ? '0' : ' ';
    if (!(flags & FLAGS_LEFT))
    {
        for (int i = space_count; i > 0; i--)
        {
            kernel_fputc(prepadding_character, stream);
            (*put_idx)++;
        }
    }

    // Sign or Space
    if (flags & FLAGS_PLUS)
    {
        kernel_fputc('+', stream);
        (*put_idx)++;
    }
    else if (flags & FLAGS_SPACE)
    {
        kernel_fputc(' ', stream);
        (*put_idx)++;
    }

    // 0x and 0
    if (flags & FLAGS_HASH)
    {
        if (base == 8)
        {
            kernel_fputc('0', stream);
            (*put_idx)++;
        }
        else if (base == 16)
        {
            kernel_fputc('0', stream);
            (*put_idx)++;

            kernel_fputc(flags & FLAGS_UPPERCASE ? 'X' : 'x', stream);
            (*put_idx)++;
        }
    }

    // Zeros
    for (int i = zeros_count; i > 0; i--)
    {
        kernel_fputc('0', stream);
        (*put_idx)++;
    }

    // Actual number
    for (int i = 0; number_buf[i] != '\0'; i++)
    {
        kernel_fputc(number_buf[i], stream);
        (*put_idx)++;
    }

    // Post padding
    if (flags & FLAGS_LEFT)
    {
        for (int i = space_count; i > 0; i--)
        {
            kernel_fputc(' ', stream);
            (*put_idx)++;
        }
    }

    heap_kernel_dealloc(number_buf);
}

void _kernel_put_signed_integer(FILE *stream, unsigned int *put_idx, i64 number, unsigned short flags, int base, int width, int precision)
{
    bool negative = false;
    if (number < 0)
    {
        number *= -1;
        negative = true;
    }

    int int_len = _kernel_unsigned_number_len(number, base); // length of the number

    char *number_buf = (char *)heap_kernel_alloc((int_len + 1) * sizeof(char), 0);
    _kernel_itoa(number, number_buf, base, flags & FLAGS_UPPERCASE, int_len);

    if (flags & FLAGS_PLUS || flags & FLAGS_SPACE || negative)
    {
        int_len += 1;
    }

    int zeros_count = precision - int_len;
    int space_count = width - _kernel_max(precision, int_len);

    // Pre padding
    char prepadding_character = flags & FLAGS_ZEROPAD ? '0' : ' ';
    if (!(flags & FLAGS_LEFT))
    {
        for (int i = space_count; i > 0; i--)
        {
            kernel_fputc(prepadding_character, stream);
            (*put_idx)++;
        }
    }

    // Sign
    if (negative)
    {
        kernel_fputc('-', stream);
        (*put_idx)++;
    }
    else if (flags & FLAGS_PLUS)
    {
        kernel_fputc('+', stream);
        (*put_idx)++;
    }
    else if (flags & FLAGS_SPACE)
    {
        kernel_fputc(' ', stream);
        (*put_idx)++;
    }

    // Zeros
    for (int i = zeros_count; i > 0; i--)
    {
        kernel_fputc('0', stream);
        (*put_idx)++;
    }

    // Actual number
    for (int i = 0; number_buf[i] != '\0'; i++)
    {
        kernel_fputc(number_buf[i], stream);
        (*put_idx)++;
    }

    // Post padding
    if (flags & FLAGS_LEFT)
    {
        for (int i = space_count; i > 0; i--)
        {
            kernel_fputc(' ', stream);
            (*put_idx)++;
        }
    }

    heap_kernel_dealloc(number_buf);
}

void _kernel_put_float(FILE *stream, unsigned int *put_idx, long double number, unsigned short flags, int width, int precision)
{
    // Print NAN
    if (isnan(number))
    {
        kernel_fputc('n', stream);
        kernel_fputc('a', stream);
        kernel_fputc('n', stream);
        (*put_idx) += 3;
        return;
    }

    // Print INF
    if (isinf(number))
    {
        char sign = number < 0 ? '-' : '+';
        kernel_fputc(sign, stream);
        kernel_fputc('i', stream);
        kernel_fputc('n', stream);
        kernel_fputc('f', stream);
        (*put_idx) += 4;
        return;
    }

    bool negative = number < 0;
    if (negative)
    {
        number *= -1;
    }

    if (!(flags & FLAGS_PRECISION))
    {
        precision = 6;
    }

    int whole_len = _kernel_fnumber_len(number);
    if (whole_len < 0)
        whole_len = 0;
    int number_len = whole_len + precision + 2; // +2 because of '.' and '\0'
    char *num_buff = (char *)heap_kernel_alloc(sizeof(char) * number_len, 0);

    int num_spaces = width - whole_len;

    num_buff = _kernel_ftoa(number, num_buff, flags, precision);

    if (negative || flags & FLAGS_PLUS || flags & FLAGS_SPACE)
    {
        num_spaces--;
    }

    if (!(flags & FLAGS_LEFT))
    {
        char pad_char = ((flags & FLAGS_ZEROPAD) ? '0' : ' ');
        for (int i = 0; i < num_spaces; i++)
        {
            kernel_fputc(pad_char, stream);
            (*put_idx)++;
        }
    }

    if (negative)
    {
        kernel_fputc('-', stream);
        (*put_idx)++;
    }
    else if (flags & FLAGS_PLUS)
    {
        kernel_fputc('+', stream);
        (*put_idx)++;
    }
    else if (flags & FLAGS_SPACE)
    {
        kernel_fputc(' ', stream);
        (*put_idx)++;
    }

    int idx = 0;
    while (num_buff[idx] != '\0')
    {
        kernel_fputc(num_buff[idx++], stream);
        (*put_idx)++;
    }

    if (flags & FLAGS_LEFT)
    {
        for (int i = 0; i < num_spaces; i++)
        {
            kernel_fputc(' ', stream);
            (*put_idx)++;
        }
    }

    heap_kernel_dealloc(num_buff);
}

void _kernel_put_scientific_notation(FILE *stream, unsigned int *put_idx, long double number, unsigned short flags, int width, int precision)
{
    int exponent = 0;

    bool negative = number < 0;
    if (negative)
    {
        number *= -1;
    }

    if (!(flags & FLAGS_PRECISION))
    {
        precision = 6;
    }

    // scale number to fit <1, 10> range
    // and specify its exponent
    while (!(number >= 1 && number < 10))
    {
        if (number < 1)
        {

            number *= 10;
            exponent--;
        }
        else
        {
            number /= 10;
            exponent++;
        }
    }
    int exponent_len = _kernel_unsigned_number_len(fabs(exponent), 10);

    int num_spaces = width - precision - 2; // -2 because of one digit before decimal point and decimal point itself
    num_spaces -= 2;                        // e notation and exponent sign
    num_spaces -= exponent_len;

    if (negative || flags & FLAGS_PLUS || flags & FLAGS_SPACE)
    {
        num_spaces--;
    }

    // Pre Padding
    if (!(flags & FLAGS_LEFT))
    {
        char pad_char = ((flags & FLAGS_ZEROPAD) ? '0' : ' ');
        for (int i = 0; i < num_spaces; i++)
        {
            kernel_fputc(pad_char, stream);
            (*put_idx)++;
        }
    }

    // SIGN
    if (negative)
    {
        kernel_fputc('-', stream);
        (*put_idx)++;
    }
    else if (flags & FLAGS_PLUS)
    {
        kernel_fputc('+', stream);
        (*put_idx)++;
    }
    else if (flags & FLAGS_SPACE)
    {
        kernel_fputc(' ', stream);
        (*put_idx)++;
    }

    // Actual number
    // We know that number must have 1 digit before decimal point
    // So we just cast it to int and make char from it
    // kernel_fputc((int)number + '0', stream);
    // (*put_idx)++;

    // if (precision != 0 || flags & FLAGS_HASH)
    // {
    //     kernel_fputc('.', stream);
    //     (*put_idx)++;
    // }

    // // Digits after decimal point
    // double integer;
    // double fract = modf(number, &integer);

    // fract *= pow(10, precision);
    // char *num_buff = (char *)malloc(sizeof(char) * (precision + 1));
    // num_buff = _itoa(fract, num_buff, 10, false, precision);

    // int idx = 0;
    // while (num_buff[idx] != '\0')
    // {
    //     kernel_fputc(num_buff[idx++], stream);
    //     (*put_idx)++;
    // }

    char *num_buff = (char *)heap_kernel_alloc(precision + 3, 0);
    num_buff = _kernel_ftoa(number, num_buff, flags, precision);
    int idx = 0;
    while (num_buff[idx] != '\0')
    {
        kernel_fputc(num_buff[idx++], stream);
        (*put_idx)++;
    }

    // Exponent
    kernel_fputc((flags & FLAGS_UPPERCASE ? 'E' : 'e'), stream);
    (*put_idx)++;

    // Exponent sign
    if (exponent < 0)
    {
        kernel_fputc('-', stream);
        (*put_idx)++;

        exponent *= -1;
    }
    else
    {
        kernel_fputc('+', stream);
        (*put_idx)++;
    }

    num_buff = heap_kernel_realloc(num_buff, sizeof(char) * (exponent_len + 1), 0);

    _kernel_itoa(exponent, num_buff, 10, false, exponent_len);
    idx = 0;
    while (num_buff[idx] != '\0')
    {
        kernel_fputc(num_buff[idx++], stream);
        (*put_idx)++;
    }

    if (flags & FLAGS_LEFT)
    {
        for (int i = 0; i < num_spaces; i++)
        {
            kernel_fputc(' ', stream);
            (*put_idx)++;
        }
    }

    heap_kernel_dealloc(num_buff);
}

unsigned int _kernel_parse_number_field(const char **str)
{
    unsigned int ret = 0;
    while (_kernel_is_digit(**str))
    {
        ret = ret * 10 + (unsigned int)(**str - '0');
        (*str)++;
    }

    return ret;
}

int kernel_vfprintf(FILE *stream, const char *format, va_list arg)
{
    unsigned int put_index = 0;

    unsigned short flags = 0;
    int width_field = 0;
    int precision_field = 0;

    const char *traverse;
    for (traverse = format; *traverse != '\0'; ++traverse)
    {

        // if travese is '%' just put character to buffer
        // else go to next character, check if it's not null
        // and then parse arguments

        if (*traverse != '%')
        {
            kernel_fputc(*traverse, stream);
            put_index++;
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
            if (_kernel_is_digit(*traverse))
            {
                width_field = _kernel_parse_number_field(&traverse);
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
                if (_kernel_is_digit(*traverse))
                {
                    precision_field = _kernel_parse_number_field(&traverse);
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
            case 'L':
                flags |= FLAGS_LONG_DOUBLE;
                ++traverse;
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
            {

                // Right padding
                if (!(flags & FLAGS_LEFT))
                {
                    for (int i = width_field - 1; i > 0; i--)
                    {
                        kernel_fputc(' ', stream);
                        put_index++;
                    }
                }

                // @INCOMPLETE: wide characters support when it will be ready
                int char_arg = va_arg(arg, int);
                kernel_fputc(char_arg, stream);

                put_index++;

                // Left padding
                if (flags & FLAGS_LEFT)
                {
                    for (int i = width_field - 1; i > 0; i--)
                    {
                        kernel_fputc(' ', stream);
                        put_index++;
                    }
                }
                break;
            }

            case 'd': // INTEGER
            case 'i':
            {
                i64 int_arg;
                if (flags & FLAGS_LONG)
                {
                    int_arg = va_arg(arg, long int);
                }
                else if (flags & FLAGS_LONG_LONG)
                {
                    int_arg = va_arg(arg, i64);
                }
                else
                {
                    int_arg = va_arg(arg, int);
                }

                _kernel_put_signed_integer(stream, &put_index, int_arg, flags, 10, width_field, precision_field);
            }
            break;

            case 'o': // OCTAL INTEGER
            {
                u64 int_arg;
                if (flags & FLAGS_LONG)
                {
                    int_arg = va_arg(arg, unsigned long int);
                }
                else if (flags & FLAGS_LONG_LONG)
                {
                    int_arg = va_arg(arg, u64);
                }
                else
                {
                    int_arg = va_arg(arg, unsigned int);
                }
                _kernel_put_unsigned_integer(stream, &put_index, int_arg, flags, 8, width_field, precision_field);
                break;
            }

            case 's': // STRING
            {
                char *str_arg = va_arg(arg, char *);
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
                        kernel_fputc(' ', stream);
                        put_index++;
                    }
                }

                // put arg to buffer
                for (int i = 0; i < str_len; i++)
                {
                    kernel_fputc(str_arg[i], stream);
                    put_index++;
                }

                // Left Padding
                if (flags & FLAGS_LEFT)
                {
                    for (int i = (width_field - str_len); i > 0; i--)
                    {
                        kernel_fputc(' ', stream);
                        put_index++;
                    }
                }
            }
            break;

            case 'X':
                flags |= FLAGS_UPPERCASE; // Uppercase X
            case 'x':                     // HEX INTEGER
            {
                u64 int_arg;
                if (flags & FLAGS_LONG)
                {
                    int_arg = va_arg(arg, unsigned long int);
                }
                else if (flags & FLAGS_LONG_LONG)
                {
                    int_arg = va_arg(arg, u64);
                }
                else
                {
                    int_arg = va_arg(arg, unsigned int);
                }
                _kernel_put_unsigned_integer(stream, &put_index, int_arg, flags, 16, width_field, precision_field);
                break;
            }

            case 'F':
            case 'f':
            {
                long double d_arg;
                if (flags & FLAGS_LONG_DOUBLE)
                {
                    d_arg = va_arg(arg, long double);
                }
                else
                {
                    d_arg = va_arg(arg, double);
                }

                _kernel_put_float(stream, &put_index, d_arg, flags, width_field, precision_field);

                break;
            }

            case 'u':
            {
                u64 int_arg;
                if (flags & FLAGS_LONG)
                {
                    int_arg = va_arg(arg, unsigned long int);
                }
                else if (flags & FLAGS_LONG_LONG)
                {
                    int_arg = va_arg(arg, u64);
                }
                else
                {
                    int_arg = va_arg(arg, unsigned int);
                }
                _kernel_put_unsigned_integer(stream, &put_index, int_arg, flags, 10, width_field, precision_field);
                break;
            }

            case 'E':
                flags |= FLAGS_UPPERCASE;
            case 'e':
            {
                long double d_arg;
                if (flags & FLAGS_LONG_DOUBLE)
                {
                    d_arg = va_arg(arg, long double);
                }
                else
                {
                    d_arg = va_arg(arg, double);
                }
                _kernel_put_scientific_notation(stream, &put_index, d_arg, flags, width_field, precision_field);
                break;
            }

            case 'G':
                flags |= FLAGS_UPPERCASE;
            case 'g':
            {
                long double d_arg;
                if (flags & FLAGS_LONG_DOUBLE)
                {
                    d_arg = va_arg(arg, long double);
                }
                else
                {
                    d_arg = va_arg(arg, double);
                }

                // if numer have 5 or more digits in the whole part
                // using scientific notation will be shorter as they share
                // precision value
                if (d_arg >= 10000.0)
                {
                    _kernel_put_scientific_notation(stream, &put_index, d_arg, flags, width_field, precision_field);
                }
                else
                {
                    _kernel_put_float(stream, &put_index, d_arg, flags, width_field, precision_field);
                }
            }
            break;

            case 'p':
            {
                void *ptr = va_arg(arg, void *);
                flags |= FLAGS_UPPERCASE;
                // Ignore those flags
                flags &= ~(FLAGS_ZEROPAD | FLAGS_HASH | FLAGS_PLUS | FLAGS_SPACE);

                precision_field = sizeof(void *) * 2;
                _kernel_put_unsigned_integer(stream, &put_index, (unsigned int)ptr, flags, 16, width_field, precision_field);
                break;
            }

            case 'n':
            {
                int *ptr = va_arg(arg, int *);

                (*ptr) = put_index;
                break;
            }

            case '%':
                kernel_fputc('%', stream);
                put_index++;

                break;

            default:
                kernel_fputc(*traverse, stream);
                put_index++;

                break;
            }
        }
    }

    return put_index - 1;
}