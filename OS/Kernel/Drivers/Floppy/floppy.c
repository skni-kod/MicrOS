#include "floppy.h"

volatile floppy_header *floppy_header_data = (floppy_header *)FLOPPY_HEADER_DATA;
volatile uint8_t *dma_buffer = (uint8_t *)DMA_ADDRESS;
volatile uint32_t time_of_last_activity = 0;
volatile bool motor_enabled = false;

volatile bool floppy_interrupt_flag = false;

bool floppy_init()
{
    // Enable timer interrupt (to check if floppy can be shut down after some time of inactivity)
    pic_enable_irq(0);
    idt_attach_interrupt_handler(0, floppy_timer_interrupt);

    time_of_last_activity = timer_get_system_clock();

    // Enable floppy interrupts
    pic_enable_irq(6);
    idt_attach_interrupt_handler(6, floppy_interrupt);

    for (int i = 0; i < 100; i++)
    {
        if (!floppy_reset())
        {
            logger_log_error("[Floppy] Reset failure. Waiting 10 ms...");
            sleep(10);
        }
        else
            break;

        if (i + 1 == 100)
        {
            logger_log_error("[Floppy] Reset timeout.");
            return false;
        }
    }

    logger_log_info("[Floppy] Reset done");

    for (int i = 0; i < 100; i++)
    {
        if (!floppy_calibrate())
        {
            logger_log_error("[Floppy] Calibration failure. Waiting 10 ms...");
            sleep(10);
        }
        else
            break;

        if (i + 1 == 100)
        {
            logger_log_error("[Floppy] Calibration timeout.");
            return false;
        }
    }

    logger_log_info("[Floppy] Calibration done");

    return true;
}

void floppy_lba_to_chs(uint16_t lba, uint8_t *head, uint8_t *track, uint8_t *sector)
{
    *head = (lba % ((*floppy_header_data).sectors_per_track * 2)) / (*floppy_header_data).sectors_per_track;
    *track = lba / ((*floppy_header_data).sectors_per_track * 2);
    *sector = lba % (*floppy_header_data).sectors_per_track + 1;
}

bool floppy_reset()
{
    // Disable floppy controller
    io_out_byte(FLOPPY_DIGITAL_OUTPUT_REGISTER, 0);

    // Enable floppy controller (reset (0x04) | DMA (0x08))
    io_out_byte(FLOPPY_DIGITAL_OUTPUT_REGISTER, 0x0C);

    // Wait for interrupt and continue reset sequence
    floppy_wait_for_interrupt();

    // Tell all connected devices that we catched the interrupt (SENSE_INTERRUPT command)
    uint8_t st0, cylinder;

    floppy_confirm_interrupt(&st0, &cylinder);
    if (st0 != 0xC0)
    {
        logger_log_error("[Floppy] Invalid ST0 after reset");
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
    for (int i = 0; i < 1000; i++)
    {
        // Get main status register and check if the last bit is set
        // If yes, data register is ready for data transfer
        if (io_in_byte(FLOPPY_MAIN_STAUTS_REGISTER) & 0x80)
        {
            return true;
        }

        sleep(10);
    }

    logger_log_error("[Floppy] Timeout while waiting for availability");
    return false;
}

bool floppy_send_command(uint8_t cmd)
{
    if (!floppy_wait_until_ready())
    {
        logger_log_error("[Floppy] Timeout while waiting for command send");
        return false;
    }

    io_out_byte(FLOPPY_DATA_REGISTER, cmd);

    return true;
}

uint8_t floppy_read_data()
{
    if (!floppy_wait_until_ready())
    {
        logger_log_error("[Floppy] Timeout while waiting for data read");
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
    //uint8_t data = 0;

    // Send SPECIFY command
    floppy_send_command(0x03);

    // S S S S H H H H
    //  S = Step Rate
    //  H = Head Unload Time
    //data = ((step_rate & 0xf) << 4) | (head_unload_time & 0xf);
    floppy_send_command(0xdf);

    // H H H H H H H DMA
    //  H = Head Load Time
    //data = (head_load_time << 1) | (dma ? 0 : 1);
    floppy_send_command(0x02);
}

bool floppy_calibrate()
{
    floppy_enable_motor();

    for (int i = 0; i < 100; i++)
    {
        // Send CALIBRATE command
        floppy_send_command(0x07);

        // Send driver number
        floppy_send_command(DEVICE_NUMBER);

        floppy_wait_for_interrupt();

        uint8_t st0, cylinder;

        floppy_confirm_interrupt(&st0, &cylinder);
        if (st0 & 0xC0)
        {
            logger_log_error("[Floppy] Invalid ST0 after calibration. Waiting 10ms...");
            sleep(10);

            continue;
        }

        if (cylinder == 0)
        {
            return true;
        }
    }

    logger_log_error("[Floppy] Calibration timeout.");
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
        logger_log_warning("[Floppy] Floppy motor enabled");
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
        logger_log_warning("[Floppy] Floppy motor disabled");
    }
}

uint8_t *floppy_read_sector(uint16_t sector)
{
    uint8_t head, track, true_sector;
    floppy_lba_to_chs(sector, &head, &track, &true_sector);

    uint8_t *ptr;
    while (ptr = floppy_do_operation_on_sector(head, track, true_sector, true), ptr == NULL)
    {
        logger_log_error("[Floppy] Read failed, waiting 100 ms");
        sleep(100);
    }

    return ptr + 0xc0000000;
}

void floppy_write_sector(uint16_t sector, uint8_t *content)
{
    uint8_t head, track, true_sector;
    floppy_lba_to_chs(sector, &head, &track, &true_sector);

    memcpy((void *)dma_buffer, content, 512);
    floppy_do_operation_on_sector(head, track, true_sector, false);
}

uint8_t *floppy_do_operation_on_sector(uint8_t head, uint8_t track, uint8_t sector, bool read)
{
    // Run floppy motor and wait some time
    floppy_enable_motor();

    for (int i = 0; i < 100; i++)
    {
        // Init DMA
        floppy_dma_init(read);

        if (!floppy_seek(track, head))
        {
            logger_log_error("[Floppy] Seek failure. Waiting 10ms...");
            sleep(10);

            continue;
        }

        // Send command to read sector
        //  0x06 or 0x05 - read or write sector command
        //  0x20 - skip deleted data address marks
        //  0x40 - double density mode
        //  0x80 - operate on both tracks of the cylinder
        floppy_send_command((read ? 0x06 : 0x05) | 0x20 | 0x40 | 0x80);

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

        uint8_t sectors_per_track = (*floppy_header_data).sectors_per_track;

        // Sectors per track
        floppy_send_command(((sector + 1) >= sectors_per_track) ? sectors_per_track : (sector + 1));

        // Length of gap (0x1B = floppy 3,5)
        floppy_send_command(0x1B);

        // Data length
        floppy_send_command(0xff);

        // Wait for interrupt
        floppy_wait_for_interrupt();

        // Read command status
        uint8_t st0 = floppy_read_data();
        uint8_t st1 = floppy_read_data();
        uint8_t st2 = floppy_read_data();
        uint8_t cylinder_data = floppy_read_data();
        uint8_t head_data = floppy_read_data();
        uint8_t sector_data = floppy_read_data();
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
            char output[80] = {' '};
            logger_log_info("[Floppy] DUMP (ST0 ST1 ST2 CD HD SD BPS T H S)");
            itoa(st0, output, 5);
            itoa(st1, output + 6, 10);
            itoa(st2, output + 12, 10);
            itoa(cylinder_data, output + 18, 10);
            itoa(head_data, output + 24, 10);
            itoa(sector_data, output + 30, 10);
            itoa(bps, output + 36, 10);
            itoa(track, output + 42, 10);
            itoa(head, output + 48, 10);
            itoa(sector, output + 54, 10);

            for (int i = 0; i < 80; i++)
            {
                if (output[i] == 0)
                {
                    output[i] = ' ';
                }
            }

            output[79] = 0;
            logger_log_info(output);

            for (int i = 0; i < 100; i++)
            {
                if (!floppy_calibrate())
                {
                    logger_log_error("[Floppy] Calibration failure. Waiting 10 ms...");
                    sleep(10);
                }

                if (i + 1 == 100)
                {
                    logger_log_error("[Floppy] Calibration timeout.");
                    return false;
                }
            }

            logger_log_info("[Floppy] Calibration done");

            logger_log_warning("[Floppy] Recalibration done, waiting 100ms...");
            sleep(100);

            continue;
        }

        return st1 == 0 ? (uint8_t *)dma_buffer : NULL;
    }

    return NULL;
}

bool floppy_seek(uint8_t cylinder, uint8_t head)
{
    uint8_t st0, interrupt_cylinder;

    for (int i = 0; i < 100; i++)
    {
        // Send seek command
        floppy_send_command(0xf);
        floppy_send_command((head << 2) | DEVICE_NUMBER);
        floppy_send_command(cylinder);

        // Wait for interrupt
        floppy_wait_for_interrupt();
        floppy_confirm_interrupt(&st0, &interrupt_cylinder);
        if (st0 & 0xC0)
        {
            logger_log_error("[Floppy] Invalid ST0 after seek. Waiting 10ms...");
            sleep(10);
        }

        if (interrupt_cylinder == cylinder)
        {
            sleep(50);
            return true;
        }
    }

    logger_log_error("[Floppy] Seek timeout.");
    return false;
}

void floppy_dma_init(bool read)
{
    // Tell DMA that we want to configure floppy (channel 2)
    io_out_byte(DMA_SINGLE_CHANNEL_MASK_REGISTER, 0x06);

    // Reset Flip-Flop register
    io_out_byte(DMA_FLIP_FLOP_RESET_REGISTER, 0xff);

    // Set buffer to the specified address
    io_out_byte(DMA_START_ADDRESS_REGISTER, (uint8_t)((uint32_t)dma_buffer & 0xff));
    io_out_byte(DMA_START_ADDRESS_REGISTER, (uint8_t)((uint32_t)dma_buffer >> 8));

    // Reset Flip-Flop register
    io_out_byte(DMA_FLIP_FLOP_RESET_REGISTER, 0xff);

    // Count to 0x01ff (number of bytes in a 3.5" floppy disk track)
    io_out_byte(DMA_COUNT_REGISTER_CHANNEL, (0xff));
    io_out_byte(DMA_COUNT_REGISTER_CHANNEL, 0x01);

    // We don't want to have external page register
    io_out_byte(DMA_EXTERNAL_PAGE_REGISTER, 0);

    // | MOD1 | MOD0 | DOWN | AUTO | TRA1 | TRA0 | SEL1 | SEL0 |
    // |  0   |  1   |  0   |  1   |  x   |  x   |  1   |  0   | = 0x56
    // MOD1, MOD0 - mode
    //  00 - transfer on demand
    //  01 - single DMA transfer
    //  10 - block DMA transfer
    //  11 - cascade mode (with another DMA controller)
    // DOWN - set order of data
    // AUTO - reset address after transfer if set
    // TRA1, TRA0 - transfer type
    //  00 - self test of the controller
    //  01 - reading from memory
    //  10 - writing to memory
    //  11 - invalid
    // SEL0, SEL1 - channel to change
    io_out_byte(DMA_MODE_REGISTER, 0x52 | (read ? 0x04 : 0x08));

    // Release channel
    io_out_byte(DMA_SINGLE_CHANNEL_MASK_REGISTER, 0x02);
}

void floppy_wait_for_interrupt()
{
    while (!floppy_interrupt_flag)
        ;
    floppy_interrupt_flag = false;
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