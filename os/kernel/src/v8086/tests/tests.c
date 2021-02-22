#include "tests.h"

#ifdef TEST_V8086
#include "stdint.h"
#include "../mod_rm_parsing.h"
#include "../drivers/serial/serial.h"
#include "../drivers/vga/vga.h"
#include "string.h"
#include "../../debug_helpers/library/kernel_stdio.h"
#include "../memory_operations.h"


void interactive_tests()
{
    v8086* machine = v8086_create_machine();
    while(true)
        {
            char d[100];
            vga_printstring("Waiting for commands!\n");
            char debug_operation = serial_receive(COM1_PORT);
            vga_printstring("Recived Byte: \n");
            vga_printchar(debug_operation);
            vga_printchar(' ');
            itoa(debug_operation, d, 10);
            vga_printstring(d);
            vga_newline();
            if(debug_operation == 0) // EXIT
                break;
            else if(debug_operation == 1) // SEND REGS
                send_regs(machine);
            else if(debug_operation == 2) // SEND SREGS
                send_sregs(machine);
            else if(debug_operation == 3) // READ BYTE
            {
                uint16_t seg;
                uint16_t off;
                seg = read_reg_16();
                off = read_reg_16();
                uint8_t mem = read_byte_from_pointer(machine->Memory, get_absolute_address(seg, off));
                serial_send(COM1_PORT, mem);
            }
            else if(debug_operation == 4) // READ WORD
            {
                uint16_t seg;
                uint16_t off;
                seg = read_reg_16();
                off = read_reg_16();
                uint16_t mem = read_word_from_pointer(machine->Memory, get_absolute_address(seg, off));
                send_reg_16(mem);
            }
            else if(debug_operation == 5) // READ DWORD
            {
                uint16_t seg;
                uint16_t off;
                seg = read_reg_16();
                off = read_reg_16();
                uint32_t mem = read_dword_from_pointer(machine->Memory, get_absolute_address(seg, off));
                send_reg_32(mem);
            }
            else if(debug_operation == 6) // GET IP
            {
                send_reg_16(machine->IP.w.ip);
            }
            else if(debug_operation == 7) // READ MEM
            {
                for(int i = 0; i < 0x100000; i++)
                    serial_send(COM1_PORT,machine->Memory[i]);
            } 
            else if(debug_operation == 8) // READ SEG
            {
                uint16_t seg = read_reg_16();        
                for(int i = 0; i < 64 * 1024; i++)
                    serial_send(COM1_PORT,machine->Memory[seg * 0x10 + i]);              
            }
            else if(debug_operation == 9)
            {
                machine->IP.w.ip = read_reg_16();
            }
            else if(debug_operation == 10)
            {
                read_regs(machine);
            }
            else if(debug_operation == 11)
            {
                read_sregs(machine);
            }
            else if(debug_operation == 12)
            {
                uint32_t seg = read_reg_16();
                uint32_t off = read_reg_16();
                uint8_t length = read_reg_8();
                uint32_t addr = seg * 0x10 + off;
                for(int i = addr; i < addr + length; i++)
                {
                    machine->Memory[i] = serial_receive(COM1_PORT);
                }
            }
            else if(debug_operation == 13)
            {
                int16_t result = parse_and_execute_instruction(machine);
                serial_send(COM1_PORT, result >> 8);
                serial_send(COM1_PORT, result & 0xFF);
            }
            else if(debug_operation == 14)
            {
                char x[20];
                for(long i = 0; i < 0x100000; i++)
                {
                    machine->Memory[i] = serial_receive(COM1_PORT);
                    if((i % 0x100) == 0){
                        kernel_sprintf(x, "%x\n", i);
                        vga_printstring(x);
                    }
                }
            }
            else{
                vga_printstring("Unknown byte: ");
                vga_printchar(debug_operation);
                vga_newline();
            }
        }
}

#endif