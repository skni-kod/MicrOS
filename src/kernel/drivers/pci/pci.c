#include <drivers/pci/pci.h>
#include <assembly/io.h>
#include <logger/logger.h>
#include <stdlib.h>
#include <klibrary/ktime.h>

static pci_device devices[PCI_DEVICE_COUNT];

pci_in_data d;
pci_device dev;

uint8_t number_of_devices = 0;

void pci_init()
{
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
        io_out_long(PCI_CONFIG_ADDRESS, data->bits);
        (*info).bits_32[i] = io_in_long(PCI_CONFIG_DATA);
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

void pci_insert_device(pci_device *dev, pci_in_data *data)
{
    for (int i = 0; i < 16; i++)
    {
        data->register_num = i;
        devices[number_of_devices].descriptors[i] = data->bits;
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
    pci_insert_device(&temp, &data);
    pci_check_bridge(&temp);
    if ((temp.header_type & 0x80) != 0)
    {
        for (int i = 1; i < 8; i++)
        {
            data.function_num = i;
            pci_get_device_info(&data, &temp);
            if (temp.vendor_id != 0xFFFF)
            {
                pci_insert_device(&temp, &data);
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

void pci_io_out(pci_device *dev, uint8_t desc_index, uint32_t value)
{
    io_out_long(PCI_CONFIG_ADDRESS, dev->descriptors[desc_index]);
    io_out_long(PCI_CONFIG_DATA, value);
}

uint32_t pci_io_in(pci_device *dev, uint8_t desc_index)
{
    io_out_long(PCI_CONFIG_ADDRESS, dev->descriptors[desc_index]);
    return io_in_long(PCI_CONFIG_DATA);
}

void pci_busmaster_set(pci_device *device,bool value){
    uint32_t pci_bus_config = pci_io_in(device, PCI_COMMAND_REGISTER);
    //clear bus master flag
    pci_bus_config &= ~(PCI_COMMAND_BUSMASTER_FLAG);
    //set new value
    pci_bus_config |= value ? (PCI_COMMAND_BUSMASTER_FLAG) : 0x0;
    pci_io_out(device, PCI_COMMAND_REGISTER, pci_bus_config);
}
void pci_print_devices(uint32_t delay){
    char buff[50];

    logger_log_info("Number of devices: ");
    uint8_t nd = pci_get_number_of_devices();
    logger_log_info(itoa(nd, buff, 10));
    logger_log_info("Devices: ");
    vga_color col;
    col.color_without_blink.background = VGA_COLOR_BLACK;
    col.color_without_blink.letter = VGA_COLOR_BLUE;
    vga_printstring_color("vendor_id ", &col);
    col.color_without_blink.letter = VGA_COLOR_GREEN;
    vga_printstring_color("device_id ", &col);
    col.color_without_blink.letter = VGA_COLOR_LIGHT_BLUE;
    vga_printstring_color("header_type ", &col);
    col.color_without_blink.letter = VGA_COLOR_LIGHT_RED;
    vga_printstring_color("class_code ", &col);
    col.color_without_blink.letter = VGA_COLOR_YELLOW;
    vga_printstring_color("subclass ", &col);
    col.color_without_blink.letter = VGA_COLOR_MAGENTA;
    vga_printstring_color("prog_if\n", &col);

    for (int i = 0; i < pci_get_number_of_devices(); i++)
    {
        pci_device *dev = pci_get_device(i);
        col.color_without_blink.letter = VGA_COLOR_BLUE;
        vga_printstring_color(itoa(dev->vendor_id, buff, 16), &col);
        vga_printchar(' ');
        col.color_without_blink.letter = VGA_COLOR_GREEN;
        vga_printstring_color(itoa(dev->device_id, buff, 16), &col);
        vga_printchar(' ');
        col.color_without_blink.letter = VGA_COLOR_LIGHT_BLUE;
        vga_printstring_color(itoa(dev->header_type, buff, 16), &col);
        vga_printchar(' ');
        col.color_without_blink.letter = VGA_COLOR_LIGHT_RED;
        vga_printstring_color(itoa(dev->class_code, buff, 16), &col);
        vga_printchar(' ');
        col.color_without_blink.letter = VGA_COLOR_YELLOW;
        vga_printstring_color(itoa(dev->subclass, buff, 16), &col);
        vga_printchar(' ');
        col.color_without_blink.letter = VGA_COLOR_MAGENTA;
        vga_printstring_color(itoa(dev->prog_if, buff, 16), &col);
        vga_printchar('\n');
    }

    if(!delay)
        while(1);
    else
        sleep(delay);
}
