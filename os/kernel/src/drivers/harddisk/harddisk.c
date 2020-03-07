#include "harddisk.h"

//! Current states of all hard drives.
extern harddisk_states harddisk_current_states;

void harddisk_init()
{
    harddisk_current_states.primary_master = (HARDDISK_STATE) __harddisk_check_presence(HARDDISK_ATA_MASTER, HARDDISK_ATA_PRIMARY_BUS, &harddisk_current_states);
    harddisk_current_states.primary_slave = (HARDDISK_STATE) __harddisk_check_presence(HARDDISK_ATA_SLAVE, HARDDISK_ATA_PRIMARY_BUS, &harddisk_current_states);
    harddisk_current_states.secondary_master = (HARDDISK_STATE) __harddisk_check_presence(HARDDISK_ATA_MASTER, HARDDISK_ATA_SECONDARY_BUS, &harddisk_current_states);
    harddisk_current_states.secondary_slave = (HARDDISK_STATE) __harddisk_check_presence(HARDDISK_ATA_SLAVE, HARDDISK_ATA_SECONDARY_BUS, &harddisk_current_states);
}

harddisk_states harddisk_get_states()
{
    return harddisk_current_states;
}

HARDDISK_STATE harddisk_get_state(HARDDISK_ATA_MASTER_SLAVE type, HARDDISK_ATA_BUS_TYPE bus)
{
    const HARDDISK_STATE *state;
    const harddisk_identify_device_data *data;
    __harddisk_get_pointers(type, bus, &state, &data);

    return *state;
}

const harddisk_identify_device_data* harddisk_get_identify_data(HARDDISK_ATA_MASTER_SLAVE type, HARDDISK_ATA_BUS_TYPE bus)
{
    const HARDDISK_STATE *state;
    const harddisk_identify_device_data *data;
    __harddisk_get_pointers(type, bus, &state, &data);

    return data;
}

char* harddisk_get_disk_serial_number_terminated(HARDDISK_ATA_MASTER_SLAVE type, HARDDISK_ATA_BUS_TYPE bus, char *buffer)
{
    const HARDDISK_STATE *state;
    const harddisk_identify_device_data *data;
    __harddisk_get_pointers(type, bus, &state, &data);

    // Copy serial number to buffer
    for(int i = 0; i < HARDDISK_SERIAL_NUMBER_LENGTH; i += 2)
    {
        buffer[i] = data->fields.serial_number[i + 1];
        buffer[i + 1] = data->fields.serial_number[i];
    }
    buffer[HARDDISK_SERIAL_NUMBER_LENGTH] = '\0';
    return buffer;
}

char* harddisk_get_disk_firmware_version_terminated(HARDDISK_ATA_MASTER_SLAVE type, HARDDISK_ATA_BUS_TYPE bus, char *buffer)
{
    const HARDDISK_STATE *state;
    const harddisk_identify_device_data *data;
    __harddisk_get_pointers(type, bus, &state, &data);

    // Copy firmware version to buffer
    for(int i = 0; i < HARDDISK_FIRMWARE_VERSION_LENGTH; i += 2)
    {
        buffer[i] = data->fields.firmware_version[i + 1];
        buffer[i + 1] = data->fields.firmware_version[i];
    }
    buffer[HARDDISK_FIRMWARE_VERSION_LENGTH] = '\0';
    // Remove additional spaces
    for(int i = HARDDISK_FIRMWARE_VERSION_LENGTH - 1; i >=0; i--)
    {
        if(buffer[i] == ' ')
        {
            buffer[i] = '\0';
        }
        else
        {
            break;
        }
    }
    return buffer;
}

char* harddisk_get_disk_model_number_terminated(HARDDISK_ATA_MASTER_SLAVE type, HARDDISK_ATA_BUS_TYPE bus, char *buffer)
{
    const HARDDISK_STATE *state;
    const harddisk_identify_device_data *data;
    __harddisk_get_pointers(type, bus, &state, &data);

    // Copy model number to buffer
    for(int i = 0; i < HARDDISK_MODEL_NUMBER_LENGTH; i += 2)
    {
        buffer[i] = data->fields.model_number[i + 1];
        buffer[i + 1] = data->fields.model_number[i];
    }
    buffer[HARDDISK_MODEL_NUMBER_LENGTH] = '\0';
    // Remove additional spaces
    for(int i = HARDDISK_MODEL_NUMBER_LENGTH - 1; i >=0; i--)
    {
        if(buffer[i] == ' ')
        {
            buffer[i] = '\0';
        }
        else
        {
            break;
        }
    }
    
    return buffer;
}

uint32_t harddisk_get_user_addressable_sectors(HARDDISK_ATA_MASTER_SLAVE type, HARDDISK_ATA_BUS_TYPE bus)
{
    const HARDDISK_STATE *state;
    const harddisk_identify_device_data *data;
    __harddisk_get_pointers(type, bus, &state, &data);
    switch(*state)
    {
        case HARDDISK_ATA_PRESENT:
            return __harddisk_ata_get_user_addressable_sectors(data);
            break;
        default:
            return 0;
    }
}

uint32_t harddisk_get_disk_space(HARDDISK_ATA_MASTER_SLAVE type, HARDDISK_ATA_BUS_TYPE bus)
{
    const HARDDISK_STATE *state;
    const harddisk_identify_device_data *data;
    __harddisk_get_pointers(type, bus, &state, &data);
    switch(*state)
    {
        case HARDDISK_ATA_PRESENT:
            return __harddisk_ata_get_disk_space(data);
            break;
        default:
            return 0;
    }
}

bool harddisk_get_is_removable_media_device(HARDDISK_ATA_MASTER_SLAVE type, HARDDISK_ATA_BUS_TYPE bus)
{
    const HARDDISK_STATE *state;
    const harddisk_identify_device_data *data;
    __harddisk_get_pointers(type, bus, &state, &data);

    return data->fields.general_configuration.removable_media_device;
}

int8_t harddisk_read_sector(HARDDISK_ATA_MASTER_SLAVE type, HARDDISK_ATA_BUS_TYPE bus, uint32_t high_lba, uint32_t low_lba, uint16_t *buffer)
{
    const HARDDISK_STATE *state;
    const harddisk_identify_device_data *data;
    __harddisk_get_pointers(type, bus, &state, &data);
    switch(*state)
    {
        case HARDDISK_ATA_PRESENT:
            return __harddisk_ata_read_sector(type, bus, high_lba, low_lba, buffer);
            break;
        default:
            return 0;
    }
}

int8_t harddisk_write_sector(HARDDISK_ATA_MASTER_SLAVE type, HARDDISK_ATA_BUS_TYPE bus, uint32_t high_lba, uint32_t low_lba, uint16_t *buffer)
{
    const HARDDISK_STATE *state;
    const harddisk_identify_device_data *data;
    __harddisk_get_pointers(type, bus, &state, &data);
    switch(*state)
    {
        case HARDDISK_ATA_PRESENT:
            return __harddisk_ata_write_sector(type, bus, high_lba, low_lba, buffer);
            break;
        default:
            return 0;
    }
}