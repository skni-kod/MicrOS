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
cpuid_0x04h __cpuid_0x04h[10];
uint8_t __cpuid_0x04h_vaild_index = 0;

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
        for(int i = 0; i < 10; i++)
        {
            __cpuid_count(CPUID_GETTHREAD_CORE_CACHE_TOPOLOGY, i, __cpuid_0x04h[i].value);
            if(__cpuid_0x04h[i].fields.eax.cache_type_field == 0)
            {
                break;
            }
            __cpuid_0x04h_vaild_index++;
        }
    }
    return 1;
}

uint32_t cpuid_get_highest_function_parameter()
{
    return __cpuid_0x00h.fields.highest_function_parameter;
}

char* cpuid_get_vendor_string(char* buffer)
{
    // Copy vendor string to buffer
    for(int i = 0; i < (int)sizeof(__cpuid_vendor_string); i++)
    {
        buffer[i] = __cpuid_vendor_string[i];
    }
    return buffer;
}

uint8_t cpuid_get_stepping_id()
{
    return __cpuid_0x01h.fields.eax.stepping_id;
}

uint8_t cpuid_get_model_id()
{
    if(__cpuid_0x01h.fields.eax.model == 0x6 || __cpuid_0x01h.fields.eax.model == 0xF)
    {
        return __cpuid_0x01h.fields.eax.extended_model_id << 4 | __cpuid_0x01h.fields.eax.model;
    }
    else
    {
        return __cpuid_0x01h.fields.eax.model;
    }
}

uint8_t cpuid_get_family_id()
{
    if(__cpuid_0x01h.fields.eax.family_id != 0xF)
    {
        return __cpuid_0x01h.fields.eax.family_id;
    }
    else
    {
        return __cpuid_0x01h.fields.eax.extended_family_id + __cpuid_0x01h.fields.eax.family_id;
    }
}

uint8_t cpuid_get_processor_type()
{
    return __cpuid_0x01h.fields.eax.procesor_type;
}

uint8_t cpuid_is_hyperthreading_available()
{
    return __cpuid_0x01h.fields.edx.htt;
}

uint8_t cpuid_number_of_logical_processors()
{
    if(cpuid_is_hyperthreading_available() == 1)
    {
        return __cpuid_0x01h.fields.ebx.max_number_of_addressable_ids;
    }
    else
    {
        return 0;
    }
    
}

uint8_t cpuid_number_of_physical_processors_cores()
{
    return __cpuid_0x04h[0].fields.eax.max_num_addressable_ids_physical + 1;
}

uint8_t cpuid_get_valid_number_cache_entries()
{
    return __cpuid_0x04h_vaild_index;
}

uint32_t cpuid_get_cache_size_in_bytes(uint8_t cache_index)
{
    if(cache_index < __cpuid_0x04h_vaild_index)
    {
        if(__cpuid_0x04h[cache_index].fields.eax.cache_type_field != 0)
        {
            return (__cpuid_0x04h[cache_index].fields.ebx.w + 1) * (__cpuid_0x04h[cache_index].fields.ebx.p + 1) *
                   (__cpuid_0x04h[cache_index].fields.ebx.l + 1) * (__cpuid_0x04h[cache_index].fields.ecx.s + 1);
        }
    }
    return 0;
}

cpuid_cache_struct cpuid_get_cache_data(uint8_t cache_index)
{
    cpuid_cache_struct cache;
    if(cache_index < __cpuid_0x04h_vaild_index)
    {
        if(__cpuid_0x04h[cache_index].fields.eax.cache_type_field != 0)
        {
            cache.size = (__cpuid_0x04h[cache_index].fields.ebx.w + 1) * (__cpuid_0x04h[cache_index].fields.ebx.p + 1) *
                         (__cpuid_0x04h[cache_index].fields.ebx.l + 1) * (__cpuid_0x04h[cache_index].fields.ecx.s + 1);
            cache.type = (cpuid_cache_type)__cpuid_0x04h[cache_index].fields.eax.cache_type_field;
            cache.level = __cpuid_0x04h[cache_index].fields.eax.cache_level;
        }
    }
    return cache;
}

const cpuid_0x00h* cpuid_get_0x00h_fields()
{
    return &__cpuid_0x00h;
}

const cpuid_0x01h* cpuid_get_0x01h_fields()
{
    return &__cpuid_0x01h;
}

const cpuid_0x04h* cpuid_get_0x04h_fields(uint8_t index)
{
    return &__cpuid_0x04h[index];
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