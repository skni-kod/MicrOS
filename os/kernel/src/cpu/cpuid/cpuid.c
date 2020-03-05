#include "cpuid.h"
#include "drivers/vga/vga.h"

cpuid_version_information __cupid_version_information;
cpuid_additional_information __cpuid_additional_information;
cpuid_features_ecx_information __cpuid_features_ecx_information;
cpuid_features_edx_information __cpuid_features_edx_information;

unsigned char __cpuid_vendor_string[13];

typedef union magic_union
{
    uint32_t value1;
    unsigned char value2[4];
} magic_union;


uint8_t cpuid_init()
{
    __cpuid_vendor();
    cpuid_features(CPUID_GETFEATURES_AND_ADDITIONAL_INFORMATION, &__cupid_version_information.value, &__cpuid_additional_information.value,
                    &__cpuid_features_ecx_information.value, &__cpuid_features_edx_information.value);
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
    return __cupid_version_information.fields.stepping_id;
}

uint8_t cpuid_get_model_id()
{
    if(__cupid_version_information.fields.model == 0x6 || __cupid_version_information.fields.model == 0xF)
    {
        return __cupid_version_information.fields.extended_model_id << 4 | __cupid_version_information.fields.model;
    }
    else
    {
        return __cupid_version_information.fields.model;
    }
}

uint8_t cpuid_get_family_id()
{
    if(__cupid_version_information.fields.family_id != 0xF)
    {
        return __cupid_version_information.fields.family_id;
    }
    else
    {
        return __cupid_version_information.fields.extended_family_id + __cupid_version_information.fields.family_id;
    }
}

uint8_t cpuid_get_processor_type()
{
    return __cupid_version_information.fields.procesor_type;
}

void __cpuid_vendor()
{
    uint32_t values[4];
    magic_union mu;
    cpuid(CPUID_GETVENDORSTRING, values);
    uint32_t temp;
    temp = values[2];
    values[2] = values[3];
    values[3] = temp;
    for(int i = 1; i < 4; i++)
    {
        mu.value1 = values[i];
        for(int j = 0; j < 4; j++)
        {
            __cpuid_vendor_string[j+4*(i-1)] = mu.value2[j];
        }
    }
    __cpuid_vendor_string[12] = '\0';
}

void printBrand()
{
    uint32_t values[4];
    magic_union mu;
    cpuid(0x80000000U, values);
    if(values[0] < 0x80000004U)
    {
        vga_printstring("CANNOT DETERMINE BRAND\n");
        return;
    }
    for(unsigned int code = 0x80000002U; code<=0x80000004U; code++)
    {
        cpuid(code, values);
        for(int i = 0; i < 4; i++)
        {
            mu.value1 = values[i];
            for(int j = 0; j<4; j++)
                vga_printchar(mu.value2[j]);
        }
    }
}