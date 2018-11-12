#include "pci.h"
#include "../../Assembly/io.h"
#include "../../Logger/logger.h"

pci_dev devices[32];
uint8_t number_of_devices = 0;

uint32_t get_register(pci_in_data data)
{
    outl(PCI_CONFIG_ADDRESS, data.bits);
    return inl(PCI_CONFIG_DATA);
}

pci_dev get_device_info(pci_in_data data)
{
    pci_dev dev;
    for(int i=0; i <= 0xF; i++)
    {
        data.register_num = i;
        dev.bits_32[i] = get_register(data);
        if((dev.bits_16[i] == 0xFFFF) && (i==0))
            break;
    }
    return dev;
}

uint8_t get_number_of_devices()
{
    return number_of_devices;
}

void pci_init()
{
    pci_in_data data;
    data.enable = 1;
    char str[7] = "000 00";
    for(uint16_t bus = 0; bus < 256; bus++)
    {
        data.bus_num = bus;
        str[0] = (bus/ 100) + 48;
        str[1] = ((bus % 100)/10) + 48;
        str[2] = (bus % 10) + 48;
        for(uint8_t device=0; device < 32; device++)
        {
            str[4] = (device/10)+48;
            str[5] = (device%10)+48;
            log_info(str);
            data.device_num = device;
            pci_dev dev = get_device_info(data);
            if(dev.vendor_id != 0xFFFF)
            {
                devices[number_of_devices] = dev;
                number_of_devices++;
            }
            if(number_of_devices >= 32)
                return;
        }
    }
}