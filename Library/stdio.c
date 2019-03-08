#include <stdarg.h>
#include "stdlib.h"

#define BUFFER_SIZE 16

int sscanf(const char *str, const char *format, ...)
{
    return 0;
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

    char *traverse;
    for(traverse = format; *traverse != '\0'; ++traverse)
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

            // Parse arguments
            switch (*traverse)
            {
            case 'c': // CHARACTER
                int_arg = va_arg(arg, int);
                str[put_index++] = int_arg;
                break;

            case 'd': // INTEGER
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