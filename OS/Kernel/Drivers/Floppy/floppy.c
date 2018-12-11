#include "floppy.h"

volatile floppy_header* floppy_header_data = (floppy_header*)FLOPPY_HEADER_DATA;
volatile uint8_t* dma_buffer = (uint8_t*)DMA_ADDRESS;
volatile uint32_t time_of_last_activity = 0;
volatile bool motor_enabled = false;

volatile bool floppy_interrupt_flag = false;
    
void floppy_init()
{
    // Enable timer interrupt (to check if floppy can be shut down after some time of inactivity)
    pic_enable_irq(0);
    idt_attach_interrupt_handler(0, floppy_timer_interrupt);

    time_of_last_activity = timer_get_system_clock();

    // Enable floppy interrupts
    pic_enable_irq(6);
    idt_attach_interrupt_handler(6, floppy_interrupt);

    if(floppy_reset() == -1)
    {
        log_error("[Floppy] Reset failure");
        return;
    }
    log_info("[Floppy] Reset done");

    if(floppy_calibrate() == -1)
    {
        log_error("[Floppy] Calibration failure");
        return;
    }
    log_info("[Floppy] Calibration done");
}

void floppy_lba_to_chs(uint16_t lba, uint8_t *head, uint8_t *track, uint8_t *sector)
{
	*head = (lba % ((*floppy_header_data).sectors_per_track * 2)) / (*floppy_header_data).sectors_per_track;
	*track = lba / ((*floppy_header_data).sectors_per_track * 2);
	*sector = lba % (*floppy_header_data).sectors_per_track + 1;
}

int8_t floppy_reset()
{
    // Disable floppy controller
    outb(FLOPPY_DIGITAL_OUTPUT_REGISTER, 0);

    // Enable floppy controller (reset (0x04) | DMA (0x08))
    outb(FLOPPY_DIGITAL_OUTPUT_REGISTER, 0x04 | 0x08);

    // Wait for interrupt and continue reset sequence
    floppy_wait_for_interrupt();

    // Tell all connected devices that we catched the interrupt (SENSE_INTERRUPT command)
    uint8_t st0, cylinder;

    floppy_confirm_interrupt(&st0, &cylinder);
    if(st0 != 0xC0)
    {
        log_error("[Floppy] Invalid ST0 after reset");
        return -1;
    }

    // Set transfer speed to 500 kb/s
    //  00 - 500 Kb/s
    //  01 - 250 Kb/s
    //  10 - 100 Kb/s
    //  11 - 1 Mb/s
    outb(FLOPPY_CONTROL_REGISTER, 0);

    // Set floppy parameters
    //  step rate = 3 ms
    //  head load time = 16 ms
    //  head unload time = 240 ms
    //  DMA = yes
    floppy_set_parameters(3, 16, 240, true);

    return 0;
}

int8_t floppy_wait_until_ready()
{
    for(int i=0; i<1000; i++)
    {
        // Get main status register and check if the last bit is set
        // If yes, data register is ready for data transfer
        if(inb(FLOPPY_MAIN_STAUTS_REGISTER) & 0x80)
        {
            return 0;
        }

        sleep(1);
    }

    log_error("[Floppy] Timeout while waiting for availability");
    return -1;
}

int8_t floppy_send_command(uint8_t cmd)
{
    if(floppy_wait_until_ready() == -1)
    {
        log_error("[Floppy] Timeout while waiting for command send");
        return -1;
    }

    outb(FLOPPY_DATA_REGISTER, cmd);

    return 0;
}
 
uint8_t floppy_read_data()
{
	if(floppy_wait_until_ready() == -1)
    {
        log_error("[Floppy] Timeout while waiting for data read");
        return 0xFF;
    }

	return inb(FLOPPY_DATA_REGISTER);
}

void floppy_confirm_interrupt(uint8_t* st0, uint8_t* cylinder)
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
	data = ((step_rate & 0xf) << 4) | (head_unload_time & 0xf);
	floppy_send_command(data);
 
    // H H H H H H H DMA
    //  H = Head Load Time
	data = (head_load_time << 1) | (dma ? 0 : 1);
	floppy_send_command(data);
}

int8_t floppy_calibrate()
{
    floppy_enable_motor();

    for(int i=0; i<1000; i++)
    {
        // Send CALIBRATE command
        floppy_send_command(0x07);

        // Send driver number
        floppy_send_command(DEVICE_NUMBER);

        floppy_wait_for_interrupt();

        uint8_t st0, cylinder;

        floppy_confirm_interrupt(&st0, &cylinder);
        if((st0 & 0x20) == 0)
        {
            log_error("[Floppy] Invalid ST0 after calibration");
        }

        if(cylinder == 0)
        {
            return 0;
        }

        sleep(1);
    }

    return -1;
}

void floppy_enable_motor()
{
    if(!motor_enabled)
    {
        // Enable floppy motor (reset (0x04) | Drive 0 Motor (0x10))
        outb(FLOPPY_DIGITAL_OUTPUT_REGISTER, 0x04 | 0x08 | 0x10);
        sleep(300);
    
        log_info("[Floppy] Motor enabled");
        motor_enabled = true;
    }

    time_of_last_activity = timer_get_system_clock();
}

void floppy_disable_motor()
{
    if(motor_enabled)
    {
        // Disable floppy motor (reset (0x04))
        outb(FLOPPY_DIGITAL_OUTPUT_REGISTER, 0x04);
    
        log_info("[Floppy] Motor disabled");
        motor_enabled = false;
    }
}

uint8_t* floppy_read_sector(uint16_t sector)
{
    uint8_t head, track, true_sector;
    floppy_lba_to_chs(sector, &head, &track, &true_sector);

    return floppy_do_operation_on_sector(head, track, true_sector, true) + 0xc0000000;
}

void floppy_write_sector(uint16_t sector, uint8_t* content)
{
    uint8_t head, track, true_sector;
    floppy_lba_to_chs(sector, &head, &track, &true_sector);

    memcpy((void*)dma_buffer, content, 512);
    floppy_do_operation_on_sector(head, track, true_sector, false);
}

uint8_t* floppy_do_operation_on_sector(uint8_t head, uint8_t track, uint8_t sector, bool read)
{
    // Run floppy motor and wait some time
    floppy_enable_motor();

    // Init DMA
    floppy_dma_init(read);

    if(floppy_seek(0, head) == -1)
    {
        log_error("[Floppy] Seek failure");
        return NULL;
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
	floppy_send_command(((sector+1)>=sectors_per_track)?sectors_per_track:(sector+1));

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
    /*uint8_t head_data = */ floppy_read_data();
    /*uint8_t sector_data = */ floppy_read_data();
    uint8_t bps = floppy_read_data();

    if(st1 & 0x80)
    {
        log_error("[Floppy] End of cylinder");
        return NULL;
    }
    if(st1 & 0x20)
    {
        log_error("[Floppy] Data error");
        return NULL;
    }
    if(st1 & 0x04)
    {
        log_error("[Floppy] No data");
        return NULL;
    }
    if(st1 & 0x02)
    {
        log_error("[Floppy] Not writable");
        return NULL;
    }
    if(st1 & 0x01)
    {
        log_error("[Floppy] Missing address mark");
        return NULL;
    }

    if(st2 & 0x40)
    {
        log_error("[Floppy] Control mask");
        return NULL;
    }
    if(st2 & 0x20)
    {
        log_error("[Floppy] Data error in data field");
        return NULL;
    }
    if(st2 & 0x10)
    {
        log_error("[Floppy] Wrong cylinder");
        return NULL;
    }
    if(st2 & 0x02)
    {
        log_error("[Floppy] Bad cylinder");
        return NULL;
    }
    if(st2 & 0x01)
    {
        log_error("[Floppy] Missing data address mark");
        return NULL;
    }

    if(bps != 0x2)
    {
        log_error("[Floppy] Invalid bps");
        return NULL;
    }

	// Confirm interrupt
	floppy_confirm_interrupt(&st0, &cylinder_data);

    return (uint8_t*)dma_buffer;
}

int floppy_seek(uint32_t cylinder, uint32_t head)
{
	uint8_t st0, interrupt_cylinder;
 
	for(int i=0; i<1000; i++)
    {
		// Send seek command
		floppy_send_command(0xf);
		floppy_send_command((head) << 2 | DEVICE_NUMBER);
		floppy_send_command(cylinder);
 
		// Wait for interrupt
		floppy_wait_for_interrupt();
		floppy_confirm_interrupt(&st0, &interrupt_cylinder);
        if((st0 & 0x20) == 0)
        {
            log_error("[Floppy] Invalid ST0 after seek");
        }
 
		if (interrupt_cylinder == cylinder)
        {
            return 0;
        }

        sleep(1);
	}
 
	return -1;
}

void floppy_dma_init(bool read)
{
    // Tell DMA that we want to configure floppy (channel 2)
    outb(DMA_SINGLE_CHANNEL_MASK_REGISTER, 0x06);

    // Reset Flip-Flop register
	outb(DMA_FLIP_FLOP_RESET_REGISTER, 0xff);

    // Set buffer to the specified address
	outb(DMA_START_ADDRESS_REGISTER, (uint8_t)((uint32_t)dma_buffer & 0xff));
	outb(DMA_START_ADDRESS_REGISTER, (uint8_t)((uint32_t)dma_buffer >> 8));

    // Reset Flip-Flop register
	outb(DMA_FLIP_FLOP_RESET_REGISTER, 0xff);

    // Count to 0x23ff (number of bytes in a 3.5" floppy disk track)
	outb(DMA_COUNT_REGISTER_CHANNEL, (0xff));
	outb(DMA_COUNT_REGISTER_CHANNEL, 0x23);

    // We don't want to have external page register
	outb(DMA_EXTERNAL_PAGE_REGISTER, 0);
    
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
    outb(DMA_MODE_REGISTER, 0x52 | (read ? 0x04 : 0x08)); 
    
    // Release channel
	outb(DMA_SINGLE_CHANNEL_MASK_REGISTER, 0x02);
}

void floppy_wait_for_interrupt()
{
    while(!floppy_interrupt_flag);
    floppy_interrupt_flag = false;
}

void floppy_interrupt()
{
    floppy_interrupt_flag = true;
}

void floppy_timer_interrupt()
{
    if(motor_enabled)
    {
        uint32_t system_clock = timer_get_system_clock();
        if(system_clock >= time_of_last_activity + IDLE_TIME)
        {
            floppy_disable_motor();
        }
    }
}