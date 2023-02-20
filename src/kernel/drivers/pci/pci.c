#include "pci.h"
#include "assembly/io.h"
#include "logger/logger.h"
#include <stdlib.h>

//maybe change this to dynamic realloc if we have more devices.
static pci_device devices[PCI_DEVICE_COUNT];

pci_cmd_address_reg d;
pci_device dev;

static uint8_t number_of_devices = 0;

void pci_init()
{
    number_of_devices = 0;
    pci_check_all_buses();
}

void pci_get_device_info(pci_cmd_address_reg *data, pci_device *dev)
{
    for (int i = 0; i < 0x4; i++)
    {
        data->register_num = i;
        io_out_long(PCI_CONFIG_ADDRESS, data->bits);
        (*dev).config.dwords[i] = io_in_long(PCI_CONFIG_DATA);
    }
    //read changing part (Devices implement 256 byte space, but bridges do not)
    for (int i = 4; i < 4 + (dev->config.header_type == 0 ? 0x3C : 0xC); i++)
    {
        data->register_num = i;
        io_out_long(PCI_CONFIG_ADDRESS, data->bits);
        (*dev).config.dwords[i] = io_in_long(PCI_CONFIG_DATA);
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

void pci_insert_device(pci_device *dev, pci_cmd_address_reg *data)
{
    devices[number_of_devices] = *dev;
    number_of_devices++;
}

void pci_check_bridge(pci_device *dev)
{
    if ((dev->config.class_code == 0x06) && (dev->config.subclass == 0x04))
    {
        uint64_t secNumber = dev->config.bytes[25];
        pci_check_bus(secNumber);
    }
}

void pci_check_device(uint16_t bus, uint16_t dev)
{
    pci_cmd_address_reg data;
    data.enable = 1;
    data.bus_num = bus;
    data.device_num = dev;
    data.register_num = 0;
    data.reserved = 0;
    data.function_num = 0;
    pci_device temp;
    temp.bus_id = bus;
    temp.dev_id = dev;
    temp.func_id = 0;
    pci_get_device_info(&data, &temp);

    if (temp.config.vendor_id == (uint16_t)0xFFFF)
        return;
    pci_insert_device(&temp, &data);
    pci_check_bridge(&temp);
    if ((temp.config.header_type & 0x80) != 0)
    {
        for (int i = 1; i < 8; i++)
        {
            data.function_num = i;
            temp.func_id = i;
            pci_get_device_info(&data, &temp);
            if (temp.config.vendor_id != 0xFFFF)
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
    if ((dev.config.header_type & 0x80) == 0)
    {
        pci_check_bus(0);
    }
    else
    {
        for (uint16_t i = 0; i < 8; i++)
        {
            d.function_num = i;
            pci_get_device_info(&d, &dev);
            if (dev.config.vendor_id == 0xFFFF)
                break;
            d.bus_num = d.function_num;
            d.function_num = 0;
            pci_check_bus(i);
        }
    }
}

static inline pci_cmd_address_reg pci_setup_cmd_address_reg(pci_device* dev, uint8_t offset)
{
    pci_cmd_address_reg regData;
    regData.always_zero = 0;
    regData.register_num = (offset >> 2) & 0x3F;
    regData.device_num = dev->dev_id;
    regData.bus_num = dev->bus_id;
    regData.function_num = dev->func_id;
    regData.reserved = 0;
    regData.enable = 1;
    return regData;
}

void pci_config_out_byte(pci_device *dev, uint8_t offset, uint8_t value)
{
    pci_cmd_address_reg regData = pci_setup_cmd_address_reg(dev, offset);
    *(dev->config.bytes + offset) = value;
    io_out_long(PCI_CONFIG_ADDRESS, regData.bits);
    io_out_long(PCI_CONFIG_DATA, dev->config.dwords[regData.register_num]);
}

void pci_config_out_word(pci_device *dev, uint8_t offset, uint16_t value)
{
    pci_cmd_address_reg regData = pci_setup_cmd_address_reg(dev, offset);
    *((uint16_t*)(dev->config.bytes + offset)) = value;
    io_out_long(PCI_CONFIG_ADDRESS, regData.bits);
    io_out_long(PCI_CONFIG_DATA, dev->config.dwords[regData.register_num]);
}

void pci_config_out_dword(pci_device *dev, uint8_t offset, uint32_t value)
{
    pci_cmd_address_reg regData = pci_setup_cmd_address_reg(dev, offset);
    io_out_long(PCI_CONFIG_ADDRESS, regData.bits);
    io_out_long(PCI_CONFIG_DATA, dev->config.dwords[regData.register_num]);
}

uint8_t pci_config_in_byte(pci_device *dev, uint8_t offset)
{
    pci_cmd_address_reg regData = pci_setup_cmd_address_reg(dev, offset);
    io_out_long(PCI_CONFIG_ADDRESS, regData.bits);
    return (uint8_t)((io_in_long(PCI_CONFIG_DATA) >> ((offset & 3) * 8)) & 0xFF);
}

uint16_t pci_config_in_word(pci_device *dev, uint8_t offset)
{
    pci_cmd_address_reg regData = pci_setup_cmd_address_reg(dev, offset);  
    io_out_long(PCI_CONFIG_ADDRESS, regData.bits);
    return (uint16_t)((io_in_long(PCI_CONFIG_DATA) >> ((offset & 2) * 8)) & 0xFFFF);
}

uint32_t pci_config_in_dword(pci_device *dev, uint8_t offset)
{
    pci_cmd_address_reg regData = pci_setup_cmd_address_reg(dev, offset);
    io_out_long(PCI_CONFIG_ADDRESS, regData.bits);
    return io_in_long(PCI_CONFIG_DATA);
}

void pci_busmaster_set(pci_device *device, bool value)
{
    //THERE IS LIMIT OF HOW MANY DEVICES CAN BE BUS MASTERS AT THE SAME TIME!
    //CHECK IN SPECIFICATION OF PCI-ISA BRIDGE AND PCI CONTROLLER WHAT THAT LIMIT IS!
    //In case of PIIX3 it is 5 devices.

    device->config.command &= ~(PCI_COMMAND_BUSMASTER_FLAG);
    device->config.command |= value ? (PCI_COMMAND_BUSMASTER_FLAG) : 0x0;
    pci_config_out_dword(device, PCI_COMMAND_REGISTER, device->config.dwords[1]);
}

void pci_print_devices()
{
    char buff[50];

    logger_log_info("Number of devices: ");
    uint8_t nd = pci_get_number_of_devices();
    logger_log_info(itoa(nd, buff, 10));
    logger_log_info("Devices: ");
    vga_printstring("BUS DEV FUNC ");
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
        vga_printstring(itoa(dev->bus_id, buff, 16));
        vga_printchar(' ');
        vga_printstring(itoa(dev->dev_id, buff, 16));
        vga_printchar(' ');
        vga_printstring(itoa(dev->func_id, buff, 16));
        vga_printchar(' ');
        col.color_without_blink.letter = VGA_COLOR_BLUE;
        vga_printstring_color(itoa(dev->config.vendor_id, buff, 16), &col);
        vga_printchar(' ');
        col.color_without_blink.letter = VGA_COLOR_GREEN;
        vga_printstring_color(itoa(dev->config.device_id, buff, 16), &col);
        vga_printchar(' ');
        col.color_without_blink.letter = VGA_COLOR_LIGHT_BLUE;
        vga_printstring_color(itoa(dev->config.header_type, buff, 16), &col);
        vga_printchar(' ');
        col.color_without_blink.letter = VGA_COLOR_LIGHT_RED;
        vga_printstring_color(itoa(dev->config.class_code, buff, 16), &col);
        vga_printchar(' ');
        col.color_without_blink.letter = VGA_COLOR_YELLOW;
        vga_printstring_color(itoa(dev->config.subclass, buff, 16), &col);
        vga_printchar(' ');
        col.color_without_blink.letter = VGA_COLOR_MAGENTA;
        vga_printstring_color(itoa(dev->config.prog_if, buff, 16), &col);
        vga_printchar('\n');
    }
}
