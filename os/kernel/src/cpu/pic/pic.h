/*
    Modified by: Jakub Przystasz
    Last modification: 19.06.2022
    Refernce documents: https://pdf1.alldatasheet.com/datasheet-pdf/view/66107/INTEL/8259A.htmls
                        http://www.brokenthorn.com/Resources/OSDevPic.html
*/

#ifndef PIC_H
#define PIC_H

#define MASTER_PIC 0x20
#define SLAVE_PIC 0xA0

#define MASTER_PIC_COMMAND MASTER_PIC
#define MASTER_PIC_DATA (MASTER_PIC + 1)

#define SLAVE_PIC_COMMAND SLAVE_PIC
#define SLAVE_PIC_DATA (SLAVE_PIC + 1)

#define MASTER_OFFSET 0x20
#define SLAVE_OFFSET 0x28

#define PIC_READ_IRR 0x0A 
#define PIC_READ_ISR 0x0B

#define PIC_EOI (1 << 5)
#define PIC_MASTER_CASCADE_LINE 0x4
#define PIC_SLAVE_CASCADE_LINE 0x2

#define ICW1_FLAG_ICW4_NEEDED (1 << 0)
#define ICW1_FLAG_ICW4_NO_NEEDED (0 << 0)
#define ICW1_FLAG_MODE_SINGLE (1 << 1)
#define ICW1_FLAG_MODE_CASCADE (0 << 1)
#define ICW1_FLAG_CALL_TRIGGER_LEVEL (1 << 3)
#define ICW1_FLAG_CALL_TRIGGER_EDGE (0 << 3)
#define ICW1_FLAG_INIT (1 << 4)

#define ICW3_FLAG_BUF (1 << 3)
#define ICW3_FLAG_NO_BUF (0 << 3)
#define ICW3_FLAG_MASTER_BUF (1 << 2)
#define ICW3_FLAG_SLAVE_BUF (0 << 2)
#define ICW3_FLAG_AOEI (1 << 1)
#define ICW3_FLAG_NO_AOEI (0 << 1)
#define ICW3_FLAG_MODE_x86 (1 << 0)
#define ICW3_FLAG_MODE_MCS (0 << 0)

#define PIC_GET_ISR __pic_get_reg(PIC_READ_ISR)
#define PIC_GET_IRR __pic_get_reg(PIC_READ_IRR)


#define PIC_READ_IRR 0x0a /* OCW3 irq ready next CMD read */
#define PIC_READ_ISR 0x0b /* OCW3 irq service next CMD read */

#define PIC_EOI 0x20

#include <stdint.h>
#include "assembly/io.h"

//! pic_init
/*
    Initializes the 8259 Programmable Interrupt Controller driver 
*/
void pic_init();

//! pic_remap
/*
    Remaps IRQ numbers, because of offset in Interrupt Descriptor Table,
    first 32 numbers are reserved for CPU exceptions
*/
void pic_remap(uint32_t master_offset, uint32_t slave_offset);

//! pic_enable_irq
/*
    Unmask interrupt in PIC. 
    For now there is 2 PIC's, so we can map up to 15 hardware interrupts
    List of irq map for x86
    0 // Programmable Interrupt Timer
    1 // Keyboard
    2 // Slave PIC
    3 // COM2
    4 // COM1
    5 // LPT2
    6 // Floppy
    7 // LPT1
    8 // CMOS
    9 // Free
    10 // Free
    11 // Network interface card
    12 // Mouse
    13 // FPU
    14 // Primary ATA Hard Disk
    15 // Secondary ATA Hard Disk
*/
void pic_enable_irq(uint8_t interrupt_number);

//! pic_enable_irq
/*
    Disable IRQ of specified number
*/
void pic_disable_irq(uint8_t interrupt_number);

//! pic_handle_irq
/*
    Function handles irq and, deals with spurious IRQs
*/
uint8_t pic_handle_irq(uint8_t interrupt_number);

//! pic_send_eoi
/*
    Tell PIC device that interrupt is handled
*/
void pic_send_eoi(uint8_t interrupt_number);

//! __pic_get_irq_reg
/*
    Returns Master and slave register values first 8 bits for master, and next for slave
*/
uint16_t __pic_get_reg(uint8_t ocw3);

//! __pic_delay
/*
    Makes delay for older hardware
*/
void pic_delay();


/*
    For very old systems waiting some time between calls with sending bytes for PIC might be / is required
    (since PIC might be too slow to actually update anything with new bytes)
    To be safe this function will wait few microseconds after writing using PIT.
*/
void pic_out_byte(uint16_t port, uint16_t value);

#endif