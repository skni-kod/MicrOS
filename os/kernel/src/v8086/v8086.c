#include "v8086.h"
#include "../memory/heap/heap.h"
#include <string.h>

#define get_absolute_address(segment, offset) (((segment) * 16) + (offset))
#define read_16_from_pointer(memory, pointer) (*((uint16_t*)(memory + pointer)))
#define write_16_to_pointer(memory, pointer, value) read_16_from_pointer(memory, pointer) = (value) 
#define read_16_from_double_pointer(memory, pointer) (*((uint16_t*)(memory + read_16_from_pointer(memory, pointer))))
#define write_16_to_double_pointer(memory, pointer, value) read_16_from_double_pointer(memory, pointer) = (value)

#define push_16(machine, value) write_16_to_pointer(machine->Memory, get_absolute_address(machine->sregs.ss, machine->regs.w.sp -= 2), value)
#define pop_16(machine, value) read_16_from_pointer(machine->Memory, get_absolute_address(machine->sregs.ss, machine->regs.w.sp += 2))

v8086* v8086_create_machine()
{
    v8086* machine = (v8086*) heap_kernel_alloc(sizeof(v8086), 0);
    if(machine == NULL) return NULL;
    memset(machine, 0, sizeof(v8086));
    machine->regs.x.flags = 0x2;
    machine->sregs.cs = 0xf000;
    machine->IP = 0xfff0;
	memcpy(machine->Memory, 0xc0000000, 0x100000);

    return machine;
}

int16_t v8086_call_function(v8086* machine)
{
    while(!(machine->IP == 0xFFFF && machine->sregs.cs == 0xFFFF))
    {
        int x = -1;
        if(x) return x;
    }
    return 0;
}

int16_t v8086_call_int(v8086* machine, int16_t num)
{
    if ((num < 0) || (num > 0xFF)) return -1;
    machine -> IP = read_16_from_double_pointer(machine->Memory, get_absolute_address(0, num * 4));
    machine -> sregs.cs = read_16_from_double_pointer(machine->Memory, get_absolute_address(0, num * 4 + 2));
    push_16(machine, machine->regs.w.flags);
    push_16(machine, 0xFFFF);
    push_16(machine, 0xFFFF);
    int x = v8086_call_function(machine);
    if(x) return -2;
    return num;
}