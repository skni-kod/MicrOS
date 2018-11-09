#include "floppy.h"

volatile floppy_header* floppy_header_data = 0x7c00;
volatile uint8_t* dma_buffer = 0x0500;

volatile bool floppy_interrupt_flag = false;
char * st0_messages[] =
{
    "normal",
    "error",
    "invalid command",
    "drive not ready"
};
    
void floppy_init()
{
    floppy_dma_init();
    
    if(floppy_reset() == -1)
    {
        log_error("[Floppy] floppy_init - reset failure");
        return;
    }
    log_info("[Floppy] floppy_init - reset done");

    if(floppy_calibrate() == -1)
    {
        log_error("[Floppy] floppy_init - calibration failure");
        return;
    }
    log_info("[Floppy] floppy_init - calibration done");

    floppy_read_sector(0, 0, 1);

    vga_printstring("F: ");
    for(int i=0; i<512; i++)
    {
        //vga_printchar(dma_buffer[i]);
    }

    vga_printchar('\n');
}

uint8_t floppy_reset()
{
    // Disable floppy controller
    outb(FLOPPY_DIGITAL_OUTPUT_REGISTER, 0);

    // Enable floppy controller (reset (0x04) | DMA (0x08))
    outb(FLOPPY_DIGITAL_OUTPUT_REGISTER, 0x0C);

    // Wait for interrupt and continue reset sequence
    floppy_wait_for_interrupt();

    // Tell all connected devices that we catched the interrupt (SENSE_INTERRUPT command)
    uint32_t st0, cylinder;

    floppy_confirm_interrupt(&st0, &cylinder);
    if(st0 != 0xC0)
    {
        log_error("[Floppy] Invalid reset");
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
}

uint8_t floppy_wait_until_ready()
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

    log_error("[Floppy] floppy_wait_until_ready - timeout");
    return -1;
}

uint8_t floppy_send_command(uint8_t cmd)
{
    if(floppy_wait_until_ready() == -1)
    {
        log_error("[Floppy] floppy_send_command - timeout");
        return -1;
    }

    outb(FLOPPY_DATA_REGISTER, cmd);
}
 
uint8_t floppy_read_data()
{
	if(floppy_wait_until_ready() == -1)
    {
        log_error("[Floppy] floppy_read_data - timeout");
        return -1;
    }

	return inb(FLOPPY_DATA_REGISTER);
}

void floppy_confirm_interrupt(uint32_t* st0, uint32_t* cylinder)
{
    // Send SENSE_INTERRUPT command
	floppy_send_command(0x08);
 
	*st0 = floppy_read_data();
	*cylinder = floppy_read_data();
}

void floppy_set_parameters(uint32_t step_rate, uint32_t head_load_time, uint32_t head_unload_time, bool dma)
{
	uint32_t data = 0;
 
    // Send SPECIFY command
	floppy_send_command(0x03);
 
    // S S S S H H H H
    //  S = Step Rate
    //  H = Head Unload Time
	data = ((step_rate & 0xf) << 4) | (head_unload_time & 0xf);
	floppy_send_command(data);
 
    // H H H H H H H DMA
    //  H = Head Load Time
	data = (head_load_time << 1) | dma;
	floppy_send_command(data);
}

uint8_t floppy_calibrate()
{
    floppy_enable_motor();

    for(int i=0; i<1000; i++)
    {
        // Send CALIBRATE command
        floppy_send_command(0x07);

        // Send driver number
        floppy_send_command(0);

        floppy_wait_for_interrupt();

        uint32_t st0, cylinder;

        floppy_confirm_interrupt(&st0, &cylinder);
        if(st0 != 0x20)
        {
            log_error("[Floppy] Invalid calibration");
            log_error(st0_messages[st0 >> 6]);
        }

        if(cylinder == 0)
        {
            floppy_disable_motor();
            return 0;
        }

        sleep(1);
    }

    floppy_disable_motor();
}

void floppy_enable_motor()
{
    // Enable floppy motor (reset (0x04) | Drive 0 Motor (0x10))
    outb(FLOPPY_DIGITAL_OUTPUT_REGISTER, 0x14);
    sleep(600);
}

void floppy_disable_motor()
{
    // Disable floppy motor (reset (0x04))
    outb(FLOPPY_DIGITAL_OUTPUT_REGISTER, 0x04);
    sleep(600);
}

void floppy_read_sector(uint8_t head, uint8_t track, uint8_t sector)
{
    // Run floppy motor and wait some time
    floppy_enable_motor();

    floppy_seek(0, head);
 
	// Prepare DMA for reading
	floppy_dma_read();
 
	// Send command to read sector
    //  0x06 - read sector command
    //  0x20 - skip deleted data address marks
    //  0x40 - double density mode
    //  0x80 - operate on both tracks of the cylinder
	floppy_send_command(0x06 | 0x20 | 0x40 | 0x80);
	
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

    // Sectors per track
	floppy_send_command((*floppy_header_data).sectors_per_track);

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
 
    if(st0 != 0x20)
    {
        log_error("[Floppy] Invalid read sector");
        log_error(st0_messages[st0 >> 6]);
    }

    if(st1 & 0x80)
    {
        log_error("[Floppy] End of cylinder");
    }
    if(st1 & 0x20)
    {
        log_error("[Floppy] Data error");
    }
    if(st1 & 0x04)
    {
        log_error("[Floppy] No data");
    }
    if(st1 & 0x02)
    {
        log_error("[Floppy] Not writable");
    }
    if(st1 & 0x01)
    {
        log_error("[Floppy] Missing address mark");
    }


    if(st2 & 0x40)
    {
        log_error("[Floppy] control mask");
    }
    if(st2 & 0x20)
    {
        log_error("[Floppy] data error in data field");
    }
    if(st2 & 0x10)
    {
        log_error("[Floppy] wrong cylinder");
    }
    if(st2 & 0x02)
    {
        log_error("[Floppy] bad cylinder");  
    }
    if(st2 & 0x01)
    {
        log_error("[Floppy] missing data address mark");  
    }

    if(bps != 0x2)
    {
        log_error("[Floppy] invalid bps");   
    }

	// Confirm interrupt
	floppy_confirm_interrupt(&st0, &cylinder_data);
    
    // Disable floppy motor and wait some time
    floppy_disable_motor();
}

int floppy_seek(uint32_t cylinder, uint32_t head) {
 
	uint32_t st0, interrupt_cylinder;
 
	for(int i=0; i<1000; i++)
    {
		// Send seek command
		floppy_send_command(0xf);
		floppy_send_command((head) << 2 | DEVICE_NUMBER);
		floppy_send_command(cylinder);
 
		// Wait for interrupt
		floppy_wait_for_interrupt();
		floppy_confirm_interrupt(&st0, &interrupt_cylinder);
        if(st0 != 0x20)
        {
            log_error("[Floppy] Invalid seek");
        }
 
		if (interrupt_cylinder == cylinder)
        {
            return 0;
        }

        sleep(1);
	}
 
	return -1;
}

void floppy_dma_init()
{
    // Enable floppy interrupts
    pic_enable_irq(6);

    // Tell DMA that we want to configure floppy (channel 2)
    outb(DMA_SINGLE_CHANNEL_MASK_REGISTER, 0x06);

    // Reset Flip-Flop register
	outb(DMA_FLIP_FLOP_RESET_REGISTER, 0xff);

    // Set buffer to 0x0500 address
	outb(DMA_START_ADDRESS_REGISTER, 0);
	outb(DMA_START_ADDRESS_REGISTER, 0x05);

    // Reset Flip-Flop register
	outb(DMA_FLIP_FLOP_RESET_REGISTER, 0xff);

    // Count to 0x23ff (number of bytes in a 3.5" floppy disk track)
	outb(DMA_COUNT_REGISTER_CHANNEL, 0xff);
	outb(DMA_COUNT_REGISTER_CHANNEL, 0x23);

    // We don't want to have external page register
	outb(DMA_EXTERNAL_PAGE_REGISTER, 0);
    
    // Release channel
	outb(DMA_SINGLE_CHANNEL_MASK_REGISTER, 0x02);
}

void floppy_dma_read() {
 
    // Tell DMA that we want to configure floppy (channel 2)
	outb(DMA_SINGLE_CHANNEL_MASK_REGISTER, 0x06);

	// | MOD1 | MOD0 | DOWN | AUTO | TRA1 | TRA0 | SEL1 | SEL0 |
    // |  0   |  1   |  0   |  1   |  0   |  1   |  1   |  0   | = 0x56
    // MOD0, MOD1 - mode
    //  00 - transfer on demand
    //  01 - single DMA transfer
    //  10 - block DMA transfer
    //  11 - cascade mode (with another DMA controller)
    // DOWN - set order of data
    // AUTO - reset address after transfer if set
    // TRA0, TRA1 - transfer type
    //  00 - self test of the controller
    //  01 - writing to memory
    //  10 - reading from memory
    //  11 - invalid
    // SEL0, SEL1 - channel to change
    outb(DMA_MODE_REGISTER, 0x56); 

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