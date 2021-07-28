#include "mouse.h"

uint8_t cycle = 0, mouse_data[3], tmp, device_id;
int32_t mouse_x, mouse_y, counter = 0;

void ps2mouse_init()
{

    // Check if PS/2 controller is available to write
    ps2mouse_check_buffer_status(Input);
    io_out_byte(0x64, 0xA8); // Enable PS/2 second port

    uint8_t configurationByte = 0;

    // reading the configuration byte
    if (ps2mouse_check_buffer_status(Input) == Empty)
    {
        io_out_byte(0x64, 0x20);
    }
    if (ps2mouse_check_buffer_status(Output) == Full)
    {
        configurationByte = io_in_byte(0x60);
    }

    // editing
    configurationByte |= 0x02;

    // writing
    if (ps2mouse_check_buffer_status(Input) == Empty)
    {
        io_out_byte(0x64, 0x60);
    }
    if (ps2mouse_check_buffer_status(Input) == Empty)
    {
        io_out_byte(0x60, configurationByte);
    }

    //get device ID
    ps2mouse_write(0xF5);
    ps2mouse_write(0xF2);
    ps2mouse_read(&device_id);

    // Set defaults
    ps2mouse_write(0xF6);

    // Enable data reporting
    ps2mouse_write(0xF4);

    pic_enable_irq(IRQ_NUM);                                     // IRQ Controller
    idt_attach_interrupt_handler(IRQ_NUM, ps2mouse_irq_handler); // CPU IDT
}

bool ps2mouse_irq_handler()
{
    static char str[10] = "";

    switch (cycle)
    {
    case 0:
        ps2mouse_read(mouse_data);
        cycle++;
        break;
    case 1:
        ps2mouse_read(mouse_data + 1);
        cycle++;
        break;
    case 2:
        ps2mouse_read(mouse_data + 2);
        mouse_x += mouse_data[1] - ((mouse_data[0] << 4) & 0x100);
        mouse_y += mouse_data[2] - ((mouse_data[0] << 3) & 0x100);
        memcpy(str,"X:", 2);
        itoa(mouse_x, str+2, 10);
        logger_log_info(str);
        memcpy(str,"Y:", 2);
        itoa(mouse_y, str+2, 10);
        logger_log_info(str);
        cycle = 0;
        break;
    }

    return false;
}

BufferStatus ps2mouse_check_buffer_status(BufferType type)
{
    uint32_t i = TIMEOUT;
    switch (type)
    {
    case Input:

        while (i)
        {
            if ((io_in_byte(0x64) & 0x02) == 0)
            {
                return Empty;
            }
            i--;
        }
        return Full;

    case Output:

        while (i)
        {
            if ((io_in_byte(0x64) & 0x01) == 1)
            {
                return Full;
            }
            i--;
        }
        return Empty;
    };
}

bool ps2mouse_write(uint8_t byte)
{
    if (ps2mouse_check_buffer_status(Input) != Empty)
    {
        return false;
    }

    io_out_byte(0x64, 0xD4);

    if (ps2mouse_check_buffer_status(Input) != Empty)
    {
        return false;
    }

    io_out_byte(0x60, byte);

    counter = PS2_MOUSE_TIMEOUT;
    while (counter-- >= 1)
    {
        if (!ps2mouse_read(&tmp))
            return false;
        if (tmp == PS2_MOUSE_CMD_ACK)
            break;
    }

    return true;
}

bool ps2mouse_read(uint8_t *byte)
{
    if (byte == NULL)
    {
        return false;
    }
    if (ps2mouse_check_buffer_status(Output) != Full)
    {
        return false;
    }

    *byte = io_in_byte(0x60);

    return true;
}

void ps2mouse_get_cursor_position()
{
}

void ps2mouse_set_cursor_position()
{
}