#include "crt0.h"

void _start(char *path, char *parameters)
{
    micros_process_set_current_process_signal_handler(signal_handler);

    setlocale(LC_ALL, "C");
    __signal_init();

    stdin = __stdio_create_stream();
    streams_set_stream_as_keyboard(stdin);

    stdout = __stdio_create_stream();
    streams_set_stream_as_console(stdout);

    stderr = __stdio_create_stream();
    streams_set_stream_as_console(stderr);

    int argc = 0;
    char **argv = parse_parameters(path, parameters, &argc);

    exit(main(argc, argv));
}

char **parse_parameters(char *path, char *parameters, int *count)
{
    char **argv = (char **)malloc(sizeof(char *));
    char *current_string = (char *)malloc(sizeof(char));
    char *parameteres_ptr = parameters;
    int current_string_length = 1;

    bool singleQuotationMarks = false;
    bool doubleQuotationMarks = false;

    argv[0] = path;
    *count = 1;

    while (true)
    {
        if ((*parameteres_ptr == 0) ||
            (*parameteres_ptr == ' ' && !singleQuotationMarks && !doubleQuotationMarks) ||
            (singleQuotationMarks && *parameteres_ptr == '\'') ||
            (doubleQuotationMarks && *parameteres_ptr == '\"'))
        {
            current_string[current_string_length - 1] = 0;

            (*count)++;
            argv = (char **)realloc(argv, sizeof(char *) * (*count));
            
            argv[(*count) - 1] = current_string;

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

    (*count)++;
    argv = (char **)realloc(argv, sizeof(char *) * (*count));
    argv[(*count) - 1] = 0;
    
    return argv;
}

void signal_handler(micros_signal_params *old_state)
{
    micros_signal_params local_old_state;

    // We do local copy of old state and frees it (because we don't know if handler will end with
    // micros_process_finish_signal_handler or some longjmp).
    memcpy(&local_old_state, old_state, sizeof(micros_signal_params));
    free(old_state);

    switch (local_old_state.interrupt_number)
    {
    case 14: //Page fault
    {
        raise_with_param(SIGSEGV, local_old_state.cr2);
        break;
    }
    }

    micros_process_finish_signal_handler(&local_old_state);
}