#include "pic.h"
#include "../../drivers/vga/vga.h"

void pic_wait_100_us()
{
	unsigned ticks = 0;

	unsigned int last_pit;

	io_out_byte(0x43, 0x0);
 
	last_pit = io_in_byte(0x40);		// Low byte
	last_pit |= io_in_byte(0x40)<<8;		// High byte

	while(ticks < 1200)
	{
		unsigned int current_pit;
		io_out_byte(0x43, 0x0);
		current_pit = io_in_byte(0x40);		// Low byte
		current_pit |= io_in_byte(0x40)<<8;		// High byte
		if(current_pit != last_pit)
		{
			last_pit = current_pit;
			ticks++;
		}
	}
}

void pic_init()
{
	// Move all interrupts from 0..7 to 32..39
	// First parameter means that IRQ0 data will have 0x20 (32) offset in IDT
	// Second parameter means that IRQ8 data will have 0x28 (40) offset in IDT
	pic_remap(0x20, 0x28);
}

//DEBUG ONLY FEATURE
//TODO: REWRITE THIS USING BETTER TIMING CALCS, USING PIT ISN'T THE BEST IDEA FOR THAT
void pic_delay(uint32_t us)
{
    unsigned ticks = 0;

	unsigned int last_pit;

	io_out_byte(0x43, 0x0);
 
	last_pit = io_in_byte(0x40);		// Low byte
	last_pit |= io_in_byte(0x40)<<8;		// High byte

	while(ticks < 400)
	{
		unsigned int current_pit;
		io_out_byte(0x43, 0x0);
		current_pit = io_in_byte(0x40);		// Low byte
		current_pit |= io_in_byte(0x40)<<8;		// High byte
		if(current_pit != last_pit)
		{
			last_pit = current_pit;
			ticks++;
		}
	}
}

// For very old systems waiting some time between calls with sending bytes for PIC might be / is required
// (since PIC might be too slow to actually update anything with new bytes)
// To be safe this function will wait few microseconds after writing using PIT.
void pic_out_byte(uint16_t port, uint16_t val)
{
    //Write as usual
    io_out_byte(port,val);
    //Wait
    //Since PIT runs at 1.193182 MHz this will wait 4 ticks, which corresponds to about 3.35 us
    pic_delay(1000);
}

void pic_remap(uint32_t master_offset, uint32_t slave_offset)
{
	// Proposition (Minus)
    // Make PIC init code read current masking for both master and slave PIC units, and restore this mask AFTER init.
    // Also make Micros old af hardware compatible, cause why not §(*￣▽￣*)§

    // Read current IRQ masks
    uint8_t master_mask = io_in_byte(MASTER_PIC_DATA);
    uint8_t slave_mask = io_in_byte(SLAVE_PIC_DATA);

    // First send INIT with ICW1 + ICW4 enabled
    // By setting it, next 3 bytes sent to MASTER_PIC_DATA will be interpreted as proper values for init. (Read about small IBM fuckup in desing to understand why we send this)
    // PS: Especially about offsets, really, IBM did fuck up there with great consequences. 
    pic_out_byte(MASTER_PIC_COMMAND, 0x11);

    // Send master unit  offset
    pic_out_byte(MASTER_PIC_DATA, master_offset);

    // Tell master unit  that it is going to have SLAVE unit on IRQ2 (4 = 0100)
    pic_out_byte(MASTER_PIC_DATA, 4);

    //Set master unit ICW4 mode to 8086, setting value 5 is incorrect, cause it seems there's no way to have 8086 mode with buffering enabled.
    pic_out_byte(MASTER_PIC_DATA, 0x01);

    // Same stuff, but for Slave unit
    // Also order of sending seems to not have any influence on actual data reading, but for ease of mind I made it in that order.
    // Order of sent bytes for each PIC unit does matter tho, so make sure it is sent in correct order.
    pic_out_byte(SLAVE_PIC_COMMAND, 0x11);

    // Send slave unit  offset
    pic_out_byte(SLAVE_PIC_DATA, slave_offset);

    // Tell slave unit it's identity (2 = 0010)
    pic_out_byte(SLAVE_PIC_DATA, 2);

    //Set slave unit ICW4 mode to 8086.
    pic_out_byte(SLAVE_PIC_DATA, 0x01);


    // Now restore retrieved masks, also we don't care about these bytes timing cause, we're not sending anything after these.
    io_out_byte(MASTER_PIC_DATA, master_mask);
    io_out_byte(SLAVE_PIC_DATA, slave_mask);

    //Commented original code for easy copying, sorry if it's long (*￣3￣)╭

    // // Send initialization sequence to the master PIC and slave PIC
	// io_out_byte(MASTER_PIC_COMMAND, 0x11);
	// pic_wait_100_us();

	// io_out_byte(SLAVE_PIC_COMMAND, 0x11);
	// pic_wait_100_us();

	// // Set master PIC offset
	// io_out_byte(MASTER_PIC_DATA, master_offset);
	// pic_wait_100_us();

	// // Set slave PIC offset
	// io_out_byte(SLAVE_PIC_DATA, slave_offset);
	// pic_wait_100_us();

	// // Tell master PIC that there is a slave PIC on the IRQ2 (4 = 0100)
	// io_out_byte(MASTER_PIC_DATA, 4);
	// pic_wait_100_us();
	// // Tell slave PIC about his role
	// io_out_byte(SLAVE_PIC_DATA, 2);
	// pic_wait_100_us();

	// // We are in 8086 mode
	// //TODO: Consider if should be like this:
	// //io_out_byte(MASTER_PIC_DATA, 0x05);
	// io_out_byte(MASTER_PIC_DATA, 0x01);
	// pic_wait_100_us();
	// io_out_byte(SLAVE_PIC_DATA, 0x01);
	// pic_wait_100_us();

	// vga_printstring("PIC WAIT START\n");
	// pic_wait_100_us();
	// vga_printstring("PIC WAIT STOP\n");

	// // Reset masks to the default values
	// io_out_byte(MASTER_PIC_DATA, 0x0);
	// io_out_byte(SLAVE_PIC_DATA, 0x0);


	//while(1);
	// Enable interrupts
	//io_enable_interrupts();
	//while(1);
}

void pic_enable_irq(uint8_t interrupt_number)
{
	uint16_t current_mask = io_in_byte(SLAVE_PIC_DATA) << 8 |
							io_in_byte(MASTER_PIC_DATA);
	current_mask &= ~(1 << interrupt_number);

	io_out_byte(MASTER_PIC_DATA, (uint8_t)current_mask);
	io_out_byte(SLAVE_PIC_DATA, (uint8_t)(current_mask >> 8));
}

void pic_disable_irq(uint8_t interrupt_number)
{
	uint16_t current_mask = io_in_byte(SLAVE_PIC_DATA) << 8 |
							io_in_byte(MASTER_PIC_DATA);
	current_mask |= 1 << interrupt_number;

	io_out_byte(MASTER_PIC_DATA, (uint8_t)current_mask);
	io_out_byte(SLAVE_PIC_DATA, (uint8_t)(current_mask >> 8));
}

void pic_confirm_master()
{
	io_out_byte(MASTER_PIC_COMMAND, 0x20);
}

void pic_confirm_master_and_slave()
{
	io_out_byte(MASTER_PIC_COMMAND, 0x20);
	io_out_byte(SLAVE_PIC_COMMAND, 0x20);
}