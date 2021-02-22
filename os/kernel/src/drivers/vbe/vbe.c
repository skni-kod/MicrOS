#include "vbe.h"
#include "../v8086/v8086.h"
#include "../v8086/memory_operations.h"
#include <stdbool.h>
#include <string.h>
#include "../memory/heap/heap.h"

v8086* machine = NULL;
bool initialized = false;

void VBE_initialize()
{
    if(!initialized)
    {
        machine = v8086_create_machine();
        if(machine == NULL) return;
        v8086_set_386_instruction_set(machine);
        initialized = true;
    }
}

void VBE_close()
{
    if(initialized)
    {
        v8086_destroy_machine(machine);
        initialized = false;
    }
}

bool VBE_is_initialized()
{
    return initialized;
}

VBEStatus VBE_check_existance_of_VESA()
{
    if(!initialized) return VBE_NO_INITAILIZED;
    machine->regs.x.ax = 0x4f03;
    v8086_call_int(machine, 0x10);
    if(machine->regs.x.ax != 0x004f) return VBE_NOT_EXIST;
    return VBE_OK;
}

VBEStatus VBE_get_svga_information(svga_information** information_struct_ptr){
    if(!initialized) return VBE_NO_INITAILIZED;
    svga_information* information_struct = heap_kernel_alloc(sizeof(svga_information), 0);
    if(information_struct == NULL) return VBE_INTERNAL_ERROR;
    machine->regs.x.ax = 0x4f00;
    machine->sregs.es = 0x0000;
    machine->regs.x.di = 0x7E00;
    int16_t status = v8086_call_int(machine, 0x10);
    if(status != 0x10) return VBE_INTERNAL_ERROR;
    if(machine->regs.h.al != 0x4f) return VBE_NOT_EXIST;
    if(machine->regs.h.ah != 0x00) return VBE_FUNCTION_FAILURE;
    *(uint32_t*)(information_struct->signature) = read_dword_from_pointer(machine->Memory, get_absolute_address(0x0000, 0x7E00));
    information_struct->signature[5] = '\0';
    information_struct->vesa_standard_number = read_word_from_pointer(machine->Memory, get_absolute_address(0x0000, 0x7E04));
    uint16_t off = read_word_from_pointer(machine->Memory, get_absolute_address(0x0000, 0x7E06));
    uint16_t seg = read_word_from_pointer(machine->Memory, get_absolute_address(0x0000, 0x7E08));
    uint16_t len = strlen(machine->Memory + get_absolute_address(seg, off));
    information_struct->producent_text = heap_kernel_alloc(len + 1, 0);
    strcpy(information_struct->producent_text, machine->Memory + get_absolute_address(seg, off));
    information_struct->additional_info = read_dword_from_pointer(machine->Memory, get_absolute_address(0x0000, 0x7E0A));
    off = read_word_from_pointer(machine->Memory, get_absolute_address(0x0000, 0x7E0E));
    seg = read_word_from_pointer(machine->Memory, get_absolute_address(0x0000, 0x7E10));
    for(len = 0; read_word_from_pointer(machine->Memory, get_absolute_address(seg, off + 2*len)) != 0xffff; ++len);
    information_struct->number_of_modes = len;
    information_struct->mode_array = heap_kernel_alloc(2 * len, 0);
    memcpy(information_struct->mode_array, machine->Memory + get_absolute_address(seg, off), len * 2);
    *information_struct_ptr = information_struct;
    return VBE_OK;
}

VBEStatus VBE_destroy_svga_information(svga_information* svga_information_ptr)
{
    heap_kernel_dealloc(svga_information_ptr->mode_array);
    heap_kernel_dealloc(svga_information_ptr->producent_text);
    heap_kernel_dealloc(svga_information_ptr);
    return VBE_OK;
}