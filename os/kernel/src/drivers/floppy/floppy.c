#include "floppy.h"

volatile uint32_t floppy_sectors_per_track;
volatile uint32_t time_of_last_activity = 0;
volatile bool motor_enabled = false;

volatile uint32_t current_cylinder = 0;
volatile uint32_t current_head = 0;

volatile bool floppy_interrupt_flag = false;

bool fdc_init()
{
    // Enable timer interrupt (to check if floppy can be shut down after some time of inactivity)
    pic_enable_irq(0);
    idt_attach_interrupt_handler(0, floppy_timer_interrupt);

    time_of_last_activity = timer_get_system_clock();

    // Enable floppy interrupts
    pic_enable_irq(6);
    idt_attach_interrupt_handler(6, floppy_interrupt);

    for (int i = 0; i < 10; i++)
    {
        if (!floppy_reset())
        {
            sleep(10);
        }
        else
        {
            break;
        }

        if (i + 1 == 10)
        {
            return false;
        }
    }
    
    return true;
}

bool floppy_init(int sectors_per_track)
{
    floppy_sectors_per_track = sectors_per_track;
    for (int i = 0; i < 10; i++)
    {
        if (!floppy_calibrate())
        {
            sleep(10);
        }
        else
        {
            break;
        }

        if (i + 1 == 10)
        {
            return false;
        }
    }

    return true;
}

bool fdc_is_present()
{
    uint8_t NMI_bit = io_in_byte(112) & 0x80;
    io_out_byte(112, 0x10 | NMI_bit);
    
    return io_in_byte(113) != 0;
}

bool floppy_is_inserted()
{
    return floppy_reset() && floppy_seek(0, 0) && floppy_read_sector(0, 1);
}

void floppy_lba_to_chs(uint16_t lba, uint8_t *head, uint8_t *track, uint8_t *sector)
{
    *head = (lba % (floppy_sectors_per_track * 2)) / floppy_sectors_per_track;
    *track = lba / (floppy_sectors_per_track * 2);
    *sector = lba % floppy_sectors_per_track + 1;
}

bool floppy_reset()
{
    // Disable floppy controller
    io_out_byte(FLOPPY_DIGITAL_OUTPUT_REGISTER, 0);

    // Enable floppy controller (reset (0x04) | DMA (0x08))
    io_out_byte(FLOPPY_DIGITAL_OUTPUT_REGISTER, 0x0C);

    // Wait for interrupt and continue reset sequence
    if(!floppy_wait_for_interrupt())
    {
        return false;
    }

    // Tell all connected devices that we catched the interrupt (SENSE_INTERRUPT command)
    uint8_t st0, cylinder;

    floppy_confirm_interrupt(&st0, &cylinder);
    if (st0 != 0xC0)
    {
        return false;
    }

    // Set transfer speed to 500 kb/s
    //  00 - 500 Kb/s
    //  01 - 250 Kb/s
    //  10 - 100 Kb/s
    //  11 - 1 Mb/s
    io_out_byte(FLOPPY_CONTROL_REGISTER, 0);

    // Set floppy parameters
    //  step rate = 3 ms
    //  head load time = 16 ms
    //  head unload time = 240 ms
    //  DMA = yes
    floppy_set_parameters(3, 16, 240, true);

    return true;
}

bool floppy_wait_until_ready()
{
    for (int i = 0; i < 100; i++)
    {
        // Get main status register and check if the last bit is set
        // If yes, data register is ready for data transfer
        if (io_in_byte(FLOPPY_MAIN_STAUTS_REGISTER) & 0x80)
        {
            return true;
        }

        sleep(1);
    }

    return false;
}

bool floppy_send_command(uint8_t cmd)
{
    if (!floppy_wait_until_ready())
    {
        return false;
    }

    io_out_byte(FLOPPY_DATA_REGISTER, cmd);
    return true;
}

uint8_t floppy_read_data()
{
    if (!floppy_wait_until_ready())
    {
        return 0xFF;
    }

    return io_in_byte(FLOPPY_DATA_REGISTER);
}

void floppy_confirm_interrupt(uint8_t *st0, uint8_t *cylinder)
{
    // Send SENSE_INTERRUPT command
    floppy_send_command(0x08);

    *st0 = floppy_read_data();
    *cylinder = floppy_read_data();
}

void floppy_set_parameters(uint32_t step_rate, uint32_t head_load_time, uint32_t head_unload_time, bool dma)
{
    uint8_t data = 0;

    // Send SPECIFY command
    floppy_send_command(0x03);

    // S S S S H H H H
    //  S = Step Rate
    //  H = Head Unload Time
    data = ((16 - step_rate) << 4) | (head_unload_time / 16);
    floppy_send_command(data);

    // H H H H H H H DMA
    //  H = Head Load Time
    data = ((head_load_time / 16) << 1) | (dma ? 0 : 1);
    floppy_send_command(data);
}

bool floppy_calibrate()
{
    floppy_enable_motor();

    for (int i = 0; i < 10; i++)
    {
        // Send CALIBRATE command
        floppy_send_command(0x07);

        // Send driver number
        floppy_send_command(DEVICE_NUMBER);

        if(!floppy_wait_for_interrupt())
        {
            return false;
        }

        uint8_t st0, cylinder;

        floppy_confirm_interrupt(&st0, &cylinder);
        if (st0 & 0xC0)
        {
            sleep(10);
            continue;
        }

        if (cylinder == 0)
        {
            current_cylinder = 0;
            current_head = 0;

            return true;
        }
    }

    return false;
}

void floppy_enable_motor()
{
    if (!motor_enabled)
    {
        // Enable floppy motor (reset (0x04) | Drive 0 Motor (0x10))
        io_out_byte(FLOPPY_DIGITAL_OUTPUT_REGISTER, 0x1C);
        sleep(500);

        motor_enabled = true;
    }

    time_of_last_activity = timer_get_system_clock();
}

void floppy_disable_motor()
{
    if (motor_enabled)
    {
        // Disable floppy motor (reset (0x04))
        io_out_byte(FLOPPY_DIGITAL_OUTPUT_REGISTER, 0x0C);

        motor_enabled = false;
    }
}

uint8_t *floppy_read_sector(int device_number, int sector)
{
    uint8_t head, track, true_sector;
    floppy_lba_to_chs(sector, &head, &track, &true_sector);

    return floppy_do_operation_on_sector(head, track, true_sector, true);
}

void floppy_write_sector(int device_number, int sector, uint8_t *content)
{
    uint8_t head, track, true_sector;
    floppy_lba_to_chs(sector, &head, &track, &true_sector);

    memcpy((void *)dma_get_buffer(), content, 512);
    floppy_do_operation_on_sector(head, track, true_sector, false);
}

uint8_t *floppy_do_operation_on_sector(uint8_t head, uint8_t track, uint8_t sector, bool read)
{
    // Run floppy motor and wait some time
    floppy_enable_motor();

    for (int i = 0; i < 10; i++)
    {
        // Init DMA
        dma_init_transfer(0x06, read);

        if (!floppy_seek(track, head))
        {
            sleep(10);
            continue;
        }

        // Send command to read sector
        //  0x06 or 0x05 - read or write sector command
        //  0x40 - double density mode
        //  0x80 - operate on both tracks of the cylinder
        floppy_send_command((read ? 0x06 : 0x05) | 0x40 | 0x80);

        // _ _ _ _ _ HEAD DEV1 DEV2
        floppy_send_command(head << 2 | DEVICE_NUMBER);

        // Track number
        floppy_send_command(track);

        // Head number
        floppy_send_command(head);

        // Sector number
        floppy_send_command(sector);

        // Sector size (2 = 512)
        floppy_send_command(2);

        uint8_t sectors_per_track = floppy_sectors_per_track;

        // Sectors per track
        floppy_send_command(((sector + 1) >= sectors_per_track) ? sectors_per_track : (sector + 1));

        // Length of gap (0x1B = floppy 3,5)
        floppy_send_command(0x1B);

        // Data length
        floppy_send_command(0xff);

        // Wait for interrupt
        if(!floppy_wait_for_interrupt())
        {
            return false;
        }

        // Read command status
        /*uint8_t st0 =*/ floppy_read_data();
        uint8_t st1 = floppy_read_data();
        uint8_t st2 = floppy_read_data();
        /*uint8_t cylinder_data =*/ floppy_read_data();
        /*uint8_t head_data =*/ floppy_read_data();
        /*uint8_t sector_data =*/ floppy_read_data();
        uint8_t bps = floppy_read_data();

        if (st1 & 0x80)
        {
            logger_log_error("[Floppy] End of cylinder");
        }
        if (st1 & 0x20)
        {
            logger_log_error("[Floppy] Data error");
        }
        if (st1 & 0x04)
        {
            logger_log_error("[Floppy] No data");
        }
        if (st1 & 0x02)
        {
            logger_log_error("[Floppy] Not writable");
        }
        if (st1 & 0x01)
        {
            logger_log_error("[Floppy] Missing address mark");
        }

        if (st2 & 0x40)
        {
            logger_log_error("[Floppy] Control mask");
        }
        if (st2 & 0x20)
        {
            logger_log_error("[Floppy] Data error in data field");
        }
        if (st2 & 0x10)
        {
            logger_log_error("[Floppy] Wrong cylinder");
        }
        if (st2 & 0x02)
        {
            logger_log_error("[Floppy] Bad cylinder");
        }
        if (st2 & 0x01)
        {
            logger_log_error("[Floppy] Missing data address mark");
        }

        if (bps != 0x2)
        {
            logger_log_error("[Floppy] Invalid bps");
        }

        if (st1 != 0 || st2 != 0)
        {
            for (int i = 0; i < 10; i++)
            {
                if (!floppy_calibrate())
                {
                    sleep(10);
                }

                if (i + 1 == 10)
                {
                    return false;
                }
            }

            sleep(100);
            continue;
        }

        return st1 == 0 ? (uint8_t *)dma_get_buffer() : NULL;
    }

    return NULL;
}

bool floppy_seek(uint8_t cylinder, uint8_t head)
{
    uint8_t st0, interrupt_cylinder;

    if (current_cylinder == cylinder && current_head == head)
    {
        return true;
    }

    for (int i = 0; i < 10; i++)
    {
        // Send seek command
        floppy_send_command(0xf);
        floppy_send_command((head << 2) | DEVICE_NUMBER);
        floppy_send_command(cylinder);

        // Wait for interrupt
        if(!floppy_wait_for_interrupt())
        {
            return false;
        }
    
        floppy_confirm_interrupt(&st0, &interrupt_cylinder);
        if (st0 & 0xC0)
        {
            sleep(10);
        }

        if (interrupt_cylinder == cylinder)
        {
            current_cylinder = cylinder;
            current_head = head;

            sleep(50);
            return true;
        }
    }

    return false;
}

bool floppy_wait_for_interrupt()
{
    for(int i = 0; i < 10; i++)
    {
        if(floppy_interrupt_flag)
        {
            floppy_interrupt_flag = false;
            return true;
        }
        
        sleep(1);
    }
    
    return false;
}

void floppy_interrupt()
{
    floppy_interrupt_flag = true;
}

void floppy_timer_interrupt()
{
    if (motor_enabled)
    {
        uint32_t system_clock = timer_get_system_clock();
        if (system_clock >= time_of_last_activity + IDLE_TIME)
        {
            floppy_disable_motor();
        }
    }
}