#ifndef HEADER_FILE
#define HEADER_FILE

#include <micros.h>
#include <stdlib.h>
#include <string.h>

// path to dir
char *path_main;
// hopefully to fix things
char *full_path;

// please init length to 0 before everything
typedef struct mnu_file_list
{

    // length of the list for function and normal use, over 65k should be enough, right?
    unsigned short int length;
    // list of paths that is gotten from the function
    char **list;
    char **names;
    // array of bools equal in size to the list, only filled when dir changes for faster drawing and as such better user experience
    bool *are_they_dirs;

} mnu_file_list;

// functions to make this thing useful

// you might want to call this function from a stage and not for all the files to avoid calculating this for more files than can be displayed to preserve performance
// Prepares additonal data about the files, pass pointer to path from list, pointer to name from names and bool from are_they_dirs
void mnu_file_list_prepare_index(mnu_file_list *list_pointer, unsigned short int index)
{

    // this monstrosity should spew out the name of the dir or file

    // first of all a search for the last '/'
    int last_slash_index = 0;
    // -2 to ignore slash after dirs
    for (last_slash_index = strlen(list_pointer->list[index]) - 2; last_slash_index > 0; last_slash_index--)
    {
        if (list_pointer->list[index][last_slash_index] == '/')
        {
            break;
        }
    }

    // size calculation for malloc
    unsigned short int name_size = strlen(list_pointer->list[index]) - last_slash_index - 1;

    // name getting

    // memory allocation, +1 for termination
    list_pointer->names[index] = malloc(sizeof(char) * (name_size + 1));

    // proper name operation
    unsigned short int current_name_index = 0;
    for (unsigned short int n = last_slash_index + 1; current_name_index < name_size; n++)
    {
        list_pointer->names[index][current_name_index] = list_pointer->list[index][n];
        current_name_index++;
    }

    // terminating character
    list_pointer->names[index][name_size] = '\0';

    // allocates memory for full path to the file / directory
    full_path = malloc(strlen(list_pointer->names[index]) + strlen(path_main) + 1);

    full_path[0] = '\0';
    printf("Other2: %s %s %s\n", path_main, full_path, list_pointer->names[index]);
    strcat(full_path, path_main);
    printf("Other3: %s %s %s\n", path_main, full_path, list_pointer->names[index]);
    strcat(full_path, list_pointer->names[index]);
    printf("Other4: %s %s %s\n", path_main, full_path, list_pointer->names[index]);

    // checks if the path contains a dir, does not work correctly as of now
    list_pointer->are_they_dirs[index] = micros_filesystem_is_directory(full_path);

    // memory leaks are bad
    free(full_path);
}

// This function fills the list with data from a given path
// Pass directories only
void mnu_file_list_fill(mnu_file_list *list_pointer)
{

    // if there was any content inside it is freed in this unholy thing
    if (list_pointer->length > 0)
    {

        for (unsigned short int n = 0; n < list_pointer->length; n++)
        {
            free(list_pointer->list[n]);
            free(list_pointer->names[n]);
        }

        free(list_pointer->list);
        free(list_pointer->names);
        free(list_pointer->are_they_dirs);
    }

    // gets the amount of elements in dir and saves it in list struct
    list_pointer->length = micros_filesystem_get_entries_count_in_directory(path_main);
    // allocates memory for the glorious list
    list_pointer->list = malloc(sizeof(char *) * list_pointer->length);
    list_pointer->names = malloc(sizeof(char *) * list_pointer->length);
    list_pointer->are_they_dirs = malloc(sizeof(bool *) * list_pointer->length);
    // gets the file list
    micros_filesystem_get_entries_in_directory(path_main, list_pointer->list);

    // interpreting the list for additional info
    for (unsigned short int n = 0; n < list_pointer->length; n++)
    {
        mnu_file_list_prepare_index(list_pointer, n);
    }
}

// deallocates all the memory oser for the list
void mnu_file_list_free(mnu_file_list *list_pointer)
{

    if (list_pointer->length > 0)
    {

        for (unsigned short int n = 0; n < list_pointer->length; n++)
        {
            free(list_pointer->list[n]);
            free(list_pointer->names[n]);
        }

        free(list_pointer->list);
        free(list_pointer->names);
        free(list_pointer->are_they_dirs);
    }
}

#endif