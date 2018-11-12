#include "pci.h"
#include "../../Assembly/io.h"
#include "../../Logger/logger.h"
#include "../../../../Library/stdlib.h"

static pci_dev devices[32];
pci_in_data d;

uint8_t number_of_devices = 0;
static char boo[50];

uint32_t get_register(pci_in_data* data)
{
    //log_warning(itoa(data->bits, boo, 2));
    outl(PCI_CONFIG_ADDRESS, data->bits);
    return inl(PCI_CONFIG_DATA);
}

void get_device_info(pci_in_data* data, pci_dev* info)
{
    for(int i=0; i <= 0xF; i++)
    {
        data->register_num = i;
        (*info).bits_32[i] = get_register(data);
        if(((*info).bits_16[i] == 0xFFFF) && (i==0))
            break;
    }
}

uint8_t get_number_of_devices()
{
    return number_of_devices;
}

pci_dev* get_device(uint8_t index)
{
    return &devices[index];
}

void pci_init()
{
    d.enable = 1;
    for(uint16_t bus = 0; bus < 256; bus++)
    {
        d.bus_num = bus;
        for(uint8_t device=0; device < 32; device++)
        {
            d.device_num = device;
            get_device_info(&d, &devices[number_of_devices]);
            if(devices[number_of_devices].vendor_id != (uint16_t)0xFFFF)
            {
                number_of_devices++;
            }
            if(number_of_devices >= 32)
                return;
        }
    }
}