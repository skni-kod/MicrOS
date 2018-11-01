#include "pic.h"

void pic_init()
{
    pic_remap(0x20, 0x28);
}

void pic_remap(int source, int destination)
{
    unsigned char a1, a2;
 
	a1 = inb(PIC1_DATA);
	a2 = inb(PIC2_DATA);
 
	outb(PIC1_COMMAND, ICW1_INIT | ICW1_ICW4);
	io_wait();
	outb(PIC2_COMMAND, ICW1_INIT | ICW1_ICW4);
	io_wait();
	outb(PIC1_DATA, source);
	io_wait();
	outb(PIC2_DATA, destination);
	io_wait();
	outb(PIC1_DATA, 4); 
	io_wait();
	outb(PIC2_DATA, 2); 
	io_wait();
 
	outb(PIC1_DATA, ICW4_8086);
	io_wait();
	outb(PIC2_DATA, ICW4_8086);
	io_wait();
 
	outb(PIC1_DATA, a1);
	outb(PIC2_DATA, a2);
}

void io_wait()
{
    __asm__ ( "outb %%al, $0x80" : : "a"(0) );
}