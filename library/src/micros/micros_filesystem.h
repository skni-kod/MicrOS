#ifndef MICROS_FILESYSTEM_H
#define MICROS_FILESYSTEM_H

#include <stdbool.h>
#include <stdint.h>
#include "micros_interrupts.h"

//! Structure representing create/modify/access time of file or directory
typedef struct micros_filesystem_time
{
    //! Year part
    uint16_t year;
    
    //! Month part
    uint8_t month;
    
    //! Day part
    uint8_t day;
    
    //! Hour part
    uint8_t hour;
    
    //! Minutes part
    uint8_t minutes;
    
    //! Seconds part
    uint8_t seconds;
} __attribute__((packed)) micros_filesystem_time;

//! Structure representing information about the specified file
typedef struct micros_filesystem_file_info
{
    //! Name of the file
    char name[32];
    
    //! Path to the file
    char path[256];
    
    //! File size
    uint32_t size;

    //! Create time
    micros_filesystem_time create_time;
    
    //! Moidify time (updated during save operation)
    micros_filesystem_time modify_time;
    
    //! Access time (updated during read operation)
    micros_filesystem_time access_time;
} micros_filesystem_file_info;

//! Structure representing information about the specified directory
typedef struct micros_filesystem_directory_info
{
    //! Name of the directory
    char name[32];
    
    //! Path to the directory
    char path[256];

    //! Create time
    micros_filesystem_time create_time;
} micros_filesystem_directory_info;

#ifdef __cplusplus
extern "C" {
#endif

//! Gets information about the file with path specified in parameter
/*!
    Gets information about the file with path specified in parameter and fills structure.
    \param path Path to the file.
    \param file_info Structure which will be filled.
    \return True if file exists and structure has been filled, otherwise false.
*/
bool micros_filesystem_get_file_info(char *path, micros_filesystem_file_info *file_info);


//! Gets information about the directory with path specified in parameter
/*!
    Gets information about the directory with path specified in parameter and fills structure.
    \param path Path to the directory.
    \param directory_info Structure which will be filled.
    \return True if directory exists and structure has been filled, otherwise false.
*/
bool micros_filesystem_get_directory_info(char *path, micros_filesystem_directory_info *directory_info);

//! Reads content from the file
/*!
    Reads content from the file and stores data in the buffer passed in the parameter.
    \param path Path to the directory.
    \param buffer Structure which will be filled.
    \param start_index Position where the data will be started read.
    \param length Structure which will be filled.
    \return True if directory exists and structure has been filled, otherwise false.
*/
bool micros_filesystem_read_file(char *path, uint8_t *buffer, uint32_t start_index, uint32_t length);
uint32_t micros_filesystem_get_entries_count_in_directory(char *path);
bool micros_filesystem_get_entries_in_directory(char *path, char **entries);
bool micros_filesystem_is_file(char *path);
bool micros_filesystem_is_directory(char *path);

bool micros_filesystem_create_file(char *path);
bool micros_filesystem_create_directory(char *path);
bool micros_filesystem_delete_file(char *path);
bool micros_filesystem_delete_directory(char *path);
bool micros_filesystem_rename_file(char *path, char *new_name);
bool micros_filesystem_rename_directory(char *path, char *new_name);
bool micros_filesystem_save_to_file(char *path, char *buffer, int size);
bool micros_filesystem_append_to_file(char *path, char *buffer, int size);

bool micros_filesystem_file_exists(char *path);
bool micros_filesystem_directory_exists(char *path);

int micros_filesystem_get_free_space();
int micros_filesystem_get_total_space();

#ifdef __cplusplus
}
#endif

#endif