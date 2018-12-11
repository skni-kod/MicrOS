#include "pci.h"
#include "../../Assembly/io.h"
#include "../../Logger/logger.h"
#include "../../../../Library/stdlib.h"

static pci_dev devices[32];
pci_in_data d;
pci_dev dev;

uint8_t number_of_devices = 0;

uint32_t get_register(pci_in_data *data)
{
    io_out_long(PCI_CONFIG_ADDRESS, data->bits);
    return io_in_long(PCI_CONFIG_DATA);
}

void get_device_info(pci_in_data *data, pci_dev *info)
{
    for (int i = 0; i <= 0xF; i++)
    {
        data->register_num = i;
        (*info).bits_32[i] = get_register(data);
        if (((*info).bits_16[i] == 0xFFFF) && (i == 0))
            break;
    }
}

uint8_t get_number_of_devices()
{
    return number_of_devices;
}

pci_dev *get_device(uint8_t index)
{
    return &devices[index];
}

void insertDevice(pci_dev *dev)
{
    for (int i = 0; i < 16; i++)
    {
        devices[number_of_devices].bits_32[i] = dev->bits_32[i];
    }
    number_of_devices++;
}

void checkPciPciBridge(pci_dev *dev)
{
    if ((dev->class_code == 0x06) && (dev->subclass == 0x04))
    {
        uint64_t secNumber = dev->bits_8[25];
        checkBus(secNumber);
    }
}

void checkDevice(uint16_t bus, uint16_t dev)
{
    pci_in_data data;
    data.enable = 1;
    data.bus_num = bus;
    data.device_num = dev;
    data.register_num = 0;
    data.reserved = 0;
    data.function_num = 0;
    pci_dev temp;
    get_device_info(&data, &temp);

    if (temp.vendor_id == (uint16_t)0xFFFF)
        return;
    insertDevice(&temp);
    checkPciPciBridge(&temp);
    if ((temp.header_type & 0x80) != 0)
    {
        for (int i = 1; i < 8; i++)
        {
            data.function_num = i;
            get_device_info(&data, &temp);
            if (temp.vendor_id != 0xFFFF)
            {
                insertDevice(&temp);
                checkPciPciBridge(&temp);
            }
        }
    }
}

void checkBus(uint16_t bus)
{
    for (uint16_t i = 0; i < 32; i++)
    {
        checkDevice(bus, i);
    }
}

void checkAllBuses()
{
    d.enable = 1;
    get_device_info(&d, &dev);
    if ((dev.header_type & 0x80) == 0)
    {
        checkBus(0);
    }
    else
    {
        for (uint16_t i = 0; i < 8; i++)
        {
            d.function_num = i;
            get_device_info(&d, &dev);
            if (dev.vendor_id == 0xFFFF)
                break;
            d.bus_num = d.function_num;
            d.function_num = 0;
            checkBus(i);
        }
    }
}

void pci_init()
{
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
    checkAllBuses();
}