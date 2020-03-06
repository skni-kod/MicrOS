#include "cpuid.h"
#include "drivers/vga/vga.h"

typedef union magic_union
{
    uint32_t value1;
    unsigned char value2[4];
} magic_union;

// EAX 0x0
cpuid_0x00h __cpuid_0x00h;
unsigned char __cpuid_vendor_string[13];

// EAX 0x1
cpuid_0x01h __cpuid_0x01h;

// EAX 0x4
cpuid_0x04h __cpuid_0x04h;

uint8_t cpuid_init()
{
    __cpuid(CPUID_GETVENDORSTRING, __cpuid_0x00h.value);
    __cpuid_get_manufacturer_string();
    if(__cpuid_0x00h.fields.highest_function_parameter >= 1)
    {
        __cpuid(CPUID_GETFEATURES_AND_ADDITIONAL_INFORMATION, __cpuid_0x01h.value);
    }
    if(__cpuid_0x00h.fields.highest_function_parameter >= 4)
    {
        // We must clear ECX registry as it's index value.
        for(int i = 0;; i++)
        {
            __cpuid_0x04h.value[2] = i;
            __cpuid(CPUID_GETTHREAD_CORE_CACHE_TOPOLOGY, __cpuid_0x04h.value);
            if(__cpuid_0x04h.fields.eax_fields.cache_type_field != 0)
            {
                break;
            }
        }
    }
    return 1;
}

char* cpuid_get_vendor_string(char* buffer)
{
    // Copy vendor string to buffer
    for(int i = 0; i < sizeof(__cpuid_vendor_string); i++)
    {
        buffer[i] = __cpuid_vendor_string[i];
    }
    return buffer;
}

uint8_t cpuid_get_stepping_id()
{
    return __cpuid_0x01h.fields.eax_fields.stepping_id;
}

uint8_t cpuid_get_model_id()
{
    if(__cpuid_0x01h.fields.eax_fields.model == 0x6 || __cpuid_0x01h.fields.eax_fields.model == 0xF)
    {
        return __cpuid_0x01h.fields.eax_fields.extended_model_id << 4 | __cpuid_0x01h.fields.eax_fields.model;
    }
    else
    {
        return __cpuid_0x01h.fields.eax_fields.model;
    }
}

uint8_t cpuid_get_family_id()
{
    if(__cpuid_0x01h.fields.eax_fields.family_id != 0xF)
    {
        return __cpuid_0x01h.fields.eax_fields.family_id;
    }
    else
    {
        return __cpuid_0x01h.fields.eax_fields.extended_family_id + __cpuid_0x01h.fields.eax_fields.family_id;
    }
}

uint8_t cpuid_get_processor_type()
{
    return __cpuid_0x01h.fields.eax_fields.procesor_type;
}

uint8_t cpuid_is_hyperthreading_available()
{
    return __cpuid_0x01h.fields.edx_fields.htt;
}

uint8_t cpuid_number_of_logical_processors()
{
    if(cpuid_is_hyperthreading_available() == 1)
    {
        return __cpuid_0x01h.fields.ebx_fields.max_number_of_addressable_ids;
    }
    else
    {
        return 0;
    }
    
}

uint8_t cpuid_number_of_physical_processors_cores()
{
    return __cpuid_0x04h.fields.eax_fields.max_num_addressable_ids_physical + 1;
}

const cpuid_0x00h* cpuid_get_0x00h_fields()
{
    return &__cpuid_0x00h;
}

const cpuid_0x01h* cpuid_get_0x01h_fields()
{
    return &__cpuid_0x01h;
}

const cpuid_0x04h* cpuid_get_0x04h_fields()
{
    return &__cpuid_0x04h;
}

// Helpers

void __cpuid_get_manufacturer_string()
{
    for(int i = 0; i <= 12; i++)
    {
        if(i < 4)
        {
            __cpuid_vendor_string[i] = __cpuid_0x00h.fields.manufacturer_id_1[i];
        }
        else if(i < 8)
        {
            __cpuid_vendor_string[i] = __cpuid_0x00h.fields.manufacturer_id_3[i % 4];
        }
        else if(i < 12)
        {
            __cpuid_vendor_string[i] = __cpuid_0x00h.fields.manufacturer_id_2[i % 4];
        }
        else
        {
            __cpuid_vendor_string[i] = '\0';
        }
    }
}

char* __cpuid_get_processor_brand(char* buffer)
{
    uint32_t values[4];
    magic_union mu;
    __cpuid(0x80000000U, values);
    if(values[0] < 0x80000004U)
    {
        return buffer;
    }
    int position = 0;
    for(unsigned int code = 0x80000002U; code<=0x80000004U; code++)
    {
        __cpuid(code, values);
        for(int i = 0; i < 4; i++)
        {
            mu.value1 = values[i];
            for(int j = 0; j<4; j++)
            {
                buffer[position] = mu.value2[j];
                position++;
            }
        }
    }
    return buffer;
}