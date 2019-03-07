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

    char *traverse;
    for (traverse = format; *traverse != '\0'; traverse++)
    {

        // put char to buffer until you see '%'
        while (*traverse != '%' && *traverse != '\0')
        {
            str[put_index++] = *traverse;

            ++traverse;
        }

        if(*traverse == '\0')
            break;

        // set pointer to character after '%'
        ++traverse;

        char buffer[BUFFER_SIZE];
        int index = 0;
        switch (*traverse)
        {
        case 'c':
            int_arg = va_arg(arg, int);
            str[put_index++] = int_arg;
            break;

        case 'd':
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

        case 'o':
            int_arg = va_arg(arg, unsigned int);
            // convert argumnrnt to string with 8 as base and put to buffer
            itoa(int_arg, buffer, 8);
            index = 0;
            while (buffer[index] != '\0' && index < BUFFER_SIZE)
            {
                str[put_index++] = buffer[index];
                index++;
            }
            break;

        case 's':
            str_arg = va_arg(arg, char *);
            // put arg to buffer
            //micros_console_print_string(str_arg);
            index = 0;
            while (str_arg[index] != '\0')
            {
                str[put_index++] = str_arg[index];
                index++;
            }
            break;

        case 'x':
            int_arg = va_arg(arg, unsigned int);
            // convert arg to string with 16 as base and put to buffer
            itoa(int_arg, buffer, 16);
            index = 0;
            while (buffer[index] != '\0' && index < BUFFER_SIZE)
            {
                str[put_index++] = buffer[index];
                index++;
            }

            break;
        }
    }

    str[put_index++] = '\0';

    va_end(arg);
    return 0;
}