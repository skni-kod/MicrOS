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
bool micros_filesystem_get_file_info(const char *path, micros_filesystem_file_info *file_info);


//! Gets information about the directory with path specified in parameter
/*!
    Gets information about the directory with path specified in parameter and fills structure.
    \param path Path to the directory.
    \param directory_info Structure which will be filled.
    \return True if directory exists and structure has been filled, otherwise false.
*/
bool micros_filesystem_get_directory_info(const char *path, micros_filesystem_directory_info *directory_info);

//! Reads content from the file
/*!
    Reads content from the file and stores data in the buffer passed in the parameter.
    \param path Path to the directory.
    \param buffer Buffer where the read data will be stored. Size must be equal or bigger than length.
    \param start_index Position where the reading will be started.
    \param length Requested size of the read data.
    \return True if file was found and data was read, otherwise false.
*/
bool micros_filesystem_read_file(const char *path, uint8_t *buffer, const uint32_t start_index, const uint32_t length);

//! Gets entries count in the directory
/*!
    Gets entries count (files and directories) in the directory.
    \param path Path to the directory.
    \return Entries count.
*/
uint32_t micros_filesystem_get_entries_count_in_directory(const char *path);

//! Gets pathes of the entries in the directory 
/*!
    Gets pathes of the entries in the directory and stores them in the passed buffer.
    \param path Path to the directory.
    \param entries Buffer where the pathes will be stored.
    \return True if the target directory was found, otherwise false.
*/
bool micros_filesystem_get_entries_in_directory(const char *path, char **entries);

//! Check if the specified entry is a file 
/*!
    Check if the specified entry is a file and returns corresponding flag.
    \param path Path to the entry.
    \return True if the specified entry is a directory, otherwise false.
*/
bool micros_filesystem_is_file(const char *path);

//! Check if the specified entry is a directory 
/*!
    Check if the specified entry is a directory and returns corresponding flag.
    \param path Path to the entry.
    \return True if the specified entry is a directory, otherwise false.
*/
bool micros_filesystem_is_directory(const char *path);

//! Creates file 
/*!
    Creates file with the specified path.
    \param path Path to the new file.
    \return True if the file has been creates with success, otherwise false.
*/
bool micros_filesystem_create_file(const char *path);

//! Creates directory 
/*!
    Creates directory with the specified path.
    \param path Path to the new directory.
    \return True if the directory has been creates with success, otherwise false.
*/
bool micros_filesystem_create_directory(const char *path);

//! Deletes file 
/*!
    Deletes file with the specified path.
    \param path Path to the file.
    \return True if the file has been deleted with success, otherwise false.
*/
bool micros_filesystem_delete_file(const char *path);

//! Deletes directory 
/*!
    Deletes directory with the specified path.
    \param path Path to the directory.
    \return True if the directory has been deleted with success, otherwise false.
*/
bool micros_filesystem_delete_directory(const char *path);

//! Renames file 
/*!
    Renames file with the specified path.
    \param path Path to the file.
    \param new_name New name of the file (Without leading path).
    \return True if the file has been renamed with success, otherwise false.
*/
bool micros_filesystem_rename_file(const char *path, const char *new_name);

//! Renames directory 
/*!
    Renames directory with the specified path.
    \param path Path to the directory.
    \param new_name New name of the directory (Without leading path).
    \return True if the directory has been renamed with success, otherwise false.
*/
bool micros_filesystem_rename_directory(const char *path, const char *new_name);

//! Saves content stored in the buffer to the specified file 
/*!
    Saves content stored in the buffer to the specified file and returns flag indicating success. If file is not empty, then old data is overwrited.
    \param path Path to the file.
    \param buffer Buffer with content to save.
    \param size Buffer size.
    \return True if the content has been saved with success, otherwise false.
*/
bool micros_filesystem_save_to_file(const char *path, const char *buffer, const int size);

//! Appends content stored in the buffer to the end of the specified file 
/*!
    Appends content stored in the buffer to the end of the specified file and returns flag indicating success.
    \param path Path to the file.
    \param buffer Buffer with content to save.
    \param size Buffer size.
    \return True if the content has been saved with success, otherwise false.
*/
bool micros_filesystem_append_to_file(const char *path, const char *buffer, const int size);

//! Checks if file with the specified path exists 
/*!
    Checks if file with the specified path exists and returns corresponding flag
    \param path Path to the file.
    \return True if the file exists, otherwise false.
*/
bool micros_filesystem_file_exists(const char *path);

//! Checks if directory with the specified path exists 
/*!
    Checks if directory with the specified path exists and returns corresponding flag
    \param path Path to the directory.
    \return True if the directory exists, otherwise false.
*/
bool micros_filesystem_directory_exists(const char *path);

//! Gets free space on the specified partition
/*!
    Gets free space on the specified partition (in bytes).
    \param partition_symbol Partition symbol.
    \return Free space in bytes.
*/
int micros_filesystem_get_free_space(const char partition_symbol);

//! Gets total space on the specified partition
/*!
    Gets total space on specified partition (in bytes).
    \param partition_symbol Partition symbol.
    \return Total space in bytes.
*/
int micros_filesystem_get_total_space(const char partition_symbol);

#ifdef __cplusplus
}
#endif

#endif