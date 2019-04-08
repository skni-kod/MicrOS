#include "cpuid.h"
#include "drivers/vga/vga.h"

uint32_t values[4];
unsigned char text[13];

typedef union magic_union
{
    uint32_t value1;
    unsigned char value2[4];
} magic_union;


void printCpu()
{
    magic_union mu;
    cpuid(0, values);
    uint32_t temp;
    temp = values[2];
    values[2] = values[3];
    values[3] = temp;
    for(int i = 1; i < 4; i++)
    {
        mu.value1 = values[i];
        for(int j = 0; j < 4; j++)
        {
            text[j+4*(i-1)] = mu.value2[j];
        }
    }
    text[12] = '\0';
    vga_printstring((char* )text);
    vga_newline();
}

void printBrand()
{
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