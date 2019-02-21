#include "crt0.h"

void _start(char *path, char *parameters)
{
    int argc = 0;
    char **argv = parse_parameters(path, parameters, &argc);

    exit(main(argc, argv));
}

char **parse_parameters(char *path, char *parameters, int *count)
{
    char **argv = (char **)malloc(sizeof(char *) * 2);
    char *current_string = (char *)malloc(sizeof(char));
    char *parameteres_ptr = parameters;
    int current_string_length = 1;

    bool singleQuotationMarks = false;
    bool doubleQuotationMarks = false;

    argv[0] = path;
    *count = 2;

    while (true)
    {
        if ((*parameteres_ptr == 0) ||
            (*parameteres_ptr == ' ' && !singleQuotationMarks && !doubleQuotationMarks) ||
            (singleQuotationMarks && *parameteres_ptr == '\'') ||
            (doubleQuotationMarks && *parameteres_ptr == '\"'))
        {
            current_string[current_string_length - 1] = 0;

            argv = (char **)realloc(argv, sizeof(char *) * (*count));
            argv[(*count) - 1] = current_string;
            (*count)++;

            if (*parameteres_ptr == 0)
            {
                break;
            }

            current_string = malloc(sizeof(char));
            current_string_length = 1;

            if (singleQuotationMarks || doubleQuotationMarks)
            {
                singleQuotationMarks = false;
                doubleQuotationMarks = false;

                parameteres_ptr++;
            }
        }
        else if (*parameteres_ptr == '\'' && !singleQuotationMarks && !doubleQuotationMarks)
        {
            singleQuotationMarks = true;
        }
        else if (*parameteres_ptr == '\"' && !singleQuotationMarks && !doubleQuotationMarks)
        {
            doubleQuotationMarks = true;
        }
        else
        {
            current_string[current_string_length - 1] = *parameteres_ptr;
            current_string_length++;

            current_string = (char *)realloc(current_string, sizeof(char) * current_string_length);
        }

        parameteres_ptr++;
    }

    argv[(*count) - 1] = 0;
    return argv;
}