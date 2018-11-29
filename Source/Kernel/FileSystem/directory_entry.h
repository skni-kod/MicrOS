#ifndef DIRECTORY_ENTRY_H
#define DIRECTORY_ENTRY_H
#pragma pack(1)

#include <stdint.h>

typedef struct directory_entry_file_attributes
{
    uint8_t readonly : 1;
    uint8_t hidden : 1;
    uint8_t system : 1;
    uint8_t volume_label : 1;
    uint8_t subdirectory : 1;
    uint8_t archive : 1;
    uint8_t device : 1;
    uint8_t reserved : 1;
} directory_entry_file_attributes;

typedef struct directory_entry_user_attribtes
{
    uint8_t read_requires_password : 1;
    uint8_t write_requires_password : 1;
    uint8_t delete_requires_password : 1;
    uint8_t reserved : 1;
    uint8_t disable_checksums : 1;
    uint8_t ignore_checksum_error : 1;
    uint8_t partial_close_default : 1;
    uint8_t modify_default_open_rules : 1;
} directory_entry_user_attribtes;

typedef struct directory_entry_time
{
    uint8_t seconds : 5;
    uint8_t minutes : 6;
    uint8_t hours : 5;
} directory_entry_time;

typedef struct directory_entry_date
{
    uint8_t day : 5;
    uint8_t month : 4;
    uint8_t year : 7;
} directory_entry_date;

typedef struct directory_entry_access_rights
{
    uint8_t owner_delete_rename_attribute_change : 1;
    uint8_t owner_execute : 1;
    uint8_t owner_write_modify : 1;
    uint8_t owner_read_copy : 1;
    uint8_t group_delete_rename_attribute_change : 1;
    uint8_t group_execute : 1;
    uint8_t group_write_modify : 1;
    uint8_t group_read_copy : 1;
    uint8_t world_delete_rename_attribute_change : 1;
    uint8_t world_execute : 1;
    uint8_t world_write_modify : 1;
    uint8_t world_read_copy : 1;
    uint8_t reserved : 4;
} directory_entry_access_rights;

typedef struct directory_entry
{
    char filename[8];
    char extension[3];

    directory_entry_file_attributes file_attributes;
    directory_entry_user_attribtes user_attributes;

    char first_character;

    directory_entry_time create_time;
    directory_entry_date create_date;

    directory_entry_date last_access_date;

    directory_entry_access_rights access_rights;

    directory_entry_time modify_time;
    directory_entry_date modify_date;

    uint16_t first_sector;
    uint32_t size;
} directory_entry;

#endif