#include "pci.h"
#include "assembly/io.h"
#include "logger/logger.h"
#include <stdlib.h>

static pci_device devices[32];
pci_in_data d;
pci_device dev;

uint8_t number_of_devices = 0;

void pci_init()
{
    // TODO: Check if these commented lines are necessary.
    /*d.enable = 1;
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
    }*/
    pci_check_all_buses();
}

uint32_t pci_get_register(pci_in_data *data)
{
    io_out_long(PCI_CONFIG_ADDRESS, data->bits);
    return io_in_long(PCI_CONFIG_DATA);
}

void pci_get_device_info(pci_in_data *data, pci_device *info)
{
    for (int i = 0; i <= 0xF; i++)
    {
        data->register_num = i;
        (*info).bits_32[i] = pci_get_register(data);
        if (((*info).bits_16[i] == 0xFFFF) && (i == 0))
            break;
    }
}

pci_device *pci_get_device(uint8_t index)
{
    return &devices[index];
}

uint8_t pci_get_number_of_devices()
{
    return number_of_devices;
}

void pci_insert_device(pci_device *dev)
{
    for (int i = 0; i < 16; i++)
    {
        devices[number_of_devices].bits_32[i] = dev->bits_32[i];
    }
    number_of_devices++;
}

void pci_check_bridge(pci_device *dev)
{
    if ((dev->class_code == 0x06) && (dev->subclass == 0x04))
    {
        uint64_t secNumber = dev->bits_8[25];
        pci_check_bus(secNumber);
    }
}

void pci_check_device(uint16_t bus, uint16_t dev)
{
    pci_in_data data;
    data.enable = 1;
    data.bus_num = bus;
    data.device_num = dev;
    data.register_num = 0;
    data.reserved = 0;
    data.function_num = 0;
    pci_device temp;
    pci_get_device_info(&data, &temp);

    if (temp.vendor_id == (uint16_t)0xFFFF)
        return;
    pci_insert_device(&temp);
    pci_check_bridge(&temp);
    if ((temp.header_type & 0x80) != 0)
    {
        for (int i = 1; i < 8; i++)
        {
            data.function_num = i;
            pci_get_device_info(&data, &temp);
            if (temp.vendor_id != 0xFFFF)
            {
                pci_insert_device(&temp);
                pci_check_bridge(&temp);
            }
        }
    }
}

void pci_check_bus(uint16_t bus)
{
    for (uint16_t i = 0; i < 32; i++)
    {
        pci_check_device(bus, i);
    }
}

void pci_check_all_buses()
{
    d.enable = 1;
    pci_get_device_info(&d, &dev);
    if ((dev.header_type & 0x80) == 0)
    {
        pci_check_bus(0);
    }
    else
    {
        for (uint16_t i = 0; i < 8; i++)
        {
            d.function_num = i;
            pci_get_device_info(&d, &dev);
            if (dev.vendor_id == 0xFFFF)
                break;
            d.bus_num = d.function_num;
            d.function_num = 0;
            pci_check_bus(i);
        }
    }
}