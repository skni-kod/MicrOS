#include "mod_rm_parsing.h"
#include <string.h>
#include "memory_operations.h"

uint8_t* get_byte_register(v8086* machine, uint8_t reg_field)
{
    switch(reg_field)
    {
        case V8086_AL:
            return &(machine->regs.h.al);
        case V8086_AH:
            return &(machine->regs.h.ah);
        case V8086_BL:
            return &(machine->regs.h.bl);
        case V8086_BH:
            return &(machine->regs.h.bh);
        case V8086_CL:
            return &(machine->regs.h.cl);
        case V8086_CH:
            return &(machine->regs.h.ch);
        case V8086_DL:
            return &(machine->regs.h.dl);
        case V8086_DH:
            return &(machine->regs.h.dh);
        default:
            return NULL;
    }
    return NULL;
}

uint16_t* get_word_register(v8086* machine, uint8_t reg_field)
{
    switch(reg_field)
    {
        case V8086_AX:
            return &(machine->regs.x.ax);
        case V8086_CX:
            return &(machine->regs.x.cx);
        case V8086_DX:
            return &(machine->regs.x.dx);
        case V8086_BX:
            return &(machine->regs.x.bx);
        case V8086_SP:
            return &(machine->regs.x.sp);
        case V8086_BP:
            return &(machine->regs.x.bp);
        case V8086_SI:
            return &(machine->regs.x.si);
        case V8086_DI:
            return &(machine->regs.x.di);
        default:
            return NULL;
    }
    return NULL;
}

uint32_t* get_dword_register(v8086* machine, uint8_t reg_field)
{
    switch(reg_field)
    {
        case V8086_EAX:
            return &(machine->regs.d.eax);
        case V8086_ECX:
            return &(machine->regs.d.ecx);
        case V8086_EDX:
            return &(machine->regs.d.edx);
        case V8086_EBX:
            return &(machine->regs.d.ebx);
        case V8086_ESP:
            return &(machine->regs.d.esp);
        case V8086_EBP:
            return &(machine->regs.d.ebp);
        case V8086_ESI:
            return &(machine->regs.d.esi);
        case V8086_EDI:
            return &(machine->regs.d.edi);
        default:
            return NULL;
    }
    return NULL;
}

void* get_variable_length_register(v8086* machine, uint8_t reg_field, uint8_t width)
{
    switch (width)
    {
        case 8:
            return get_byte_register(machine, reg_field);
        case 16:
            return get_word_register(machine, reg_field);
        case 32:
            return get_dword_register(machine, reg_field);
        default:
            return NULL;
    }
}

uint16_t* select_segment_register(v8086* machine, segment_register_select select)
{
    switch(select)
    {
        case V8086_CS:
            return &(machine->sregs.cs);
        case V8086_DS:
            return &(machine->sregs.ds);
        case V8086_SS:
            return &(machine->sregs.ss);
        case V8086_ES:
            return &(machine->sregs.es);
        case V8086_FS:
            return &(machine->sregs.fs);
        case V8086_GS:
            return &(machine->sregs.gs);
        default:
            return NULL;
    }
}

int16_t calculate_segment_offset_from_mode(v8086* machine, uint8_t mod_rm, uint16_t* segment, uint32_t* offset)
{
    uint16_t* segment_register = NULL;
    if(machine->internal_state.segment_reg_select != V8086_DEFAULT)
        segment_register = select_segment_register(machine, machine->internal_state.segment_reg_select);
    else
    {
        switch (mod_rm & 7u)
        {
            case 0:
            case 1:
            case 4:
            case 5:
            case 7:
                segment_register = select_segment_register(machine, V8086_DS);
                break;
            case 6:
                if((mod_rm >> 6u) == 0) segment_register = select_segment_register(machine, V8086_DS);
                else segment_register = select_segment_register(machine, V8086_SS);
                break;
            default:
                segment_register = select_segment_register(machine, V8086_SS);
                break;
        }
    }

    if(segment_register == NULL) return V8086_UNDEFINED_SEGMENT_REGISTER;

    *segment = *segment_register;

    switch(mod_rm >> 6u) //Parsing mod than parsing rm
    {
        case 0:
            switch(mod_rm & 7u){
                case 0:
                    *offset = (uint16_t)(machine->regs.x.bx + machine->regs.x.si);
                    return V8086_OK;
                case 1:
                    *offset = (uint16_t)(machine->regs.x.bx + machine->regs.x.di);
                    return V8086_OK;
                case 2:
                    *offset = (uint16_t)(machine->regs.x.bp + machine->regs.x.si);
                    return V8086_OK;
                case 3:
                    *offset = (uint16_t)(machine->regs.x.bp + machine->regs.x.di);
                    return V8086_OK;
                case 4:
                    *offset = (uint16_t)(machine->regs.x.si);
                    return V8086_OK;
                case 5:
                    *offset = (uint16_t)(machine->regs.x.di);
                    return V8086_OK;
                case 6:{
                    *offset = (uint16_t)(read_word_from_pointer(machine->Memory, get_absolute_address(machine->sregs.cs, machine->IP.w.ip + (machine->internal_state.IPOffset))));
                    machine->internal_state.IPOffset += 2;
                    return V8086_OK;
                }
                case 7:
                    *offset = machine->regs.x.bx;
                    return V8086_OK;
                default:
                    return V8086_BAD_RM;
            }
        case 1:{
            int8_t disp = (int8_t) read_byte_from_pointer(machine->Memory, get_absolute_address(machine->sregs.cs, machine->IP.w.ip + machine->internal_state.IPOffset));
            machine->internal_state.IPOffset += 1;
            switch(mod_rm & 7u){
                case 0:
                    *offset = (uint16_t)(machine->regs.x.bx + machine->regs.x.si + disp);
                    return V8086_OK;
                case 1:
                    *offset = (uint16_t)(machine->regs.x.bx + machine->regs.x.di + disp);
                    return V8086_OK;
                case 2:
                    *offset = (uint16_t)(machine->regs.x.bp + machine->regs.x.si + disp);
                    return V8086_OK;
                case 3:
                    *offset = (uint16_t)(machine->regs.x.bp + machine->regs.x.di + disp);
                    return V8086_OK;
                case 4:
                    *offset = (uint16_t)(machine->regs.x.si + disp);
                    return V8086_OK;
                case 5:
                    *offset = (uint16_t)(machine->regs.x.di + disp);
                    return V8086_OK;
                case 6:
                    *offset = (uint16_t)(machine->regs.x.bp + disp);
                    return V8086_OK;
                case 7:
                    *offset = (uint16_t)(machine->regs.x.bx + disp);
                    return V8086_OK;
                default:
                    return V8086_BAD_RM;
            }
        }
        case 2:{
            uint16_t disp = (uint16_t) read_word_from_pointer(machine->Memory, get_absolute_address(machine->sregs.cs, machine->IP.w.ip + machine->internal_state.IPOffset));
            machine->internal_state.IPOffset += 2;
            switch(mod_rm & 7u){
                case 0:
                    *offset = (uint16_t)(machine->regs.x.bx + machine->regs.x.si + disp);
                    return V8086_OK;
                case 1:
                    *offset = (uint16_t)(machine->regs.x.bx + machine->regs.x.di + disp);
                    return V8086_OK;
                case 2:
                    *offset = (uint16_t)(machine->regs.x.bp + machine->regs.x.si + disp);
                    return V8086_OK;
                case 3:
                    *offset = (uint16_t)(machine->regs.x.bp + machine->regs.x.di + disp);
                    return V8086_OK;
                case 4:
                    *offset = (uint16_t)(machine->regs.x.si + disp);
                    return V8086_OK;
                case 5:
                    *offset = (uint16_t)(machine->regs.x.di + disp);
                    return V8086_OK;
                case 6:
                    *offset = (uint16_t)(machine->regs.x.bp + disp);
                    return V8086_OK;
                case 7:
                    *offset = (uint16_t)(machine->regs.x.bx + disp);
                    return V8086_OK;
                default:
                    return V8086_BAD_RM;
            }
        }
        default:
            return V8086_BAD_MOD;
    }
    return V8086_UNKNOWN_ERROR;
}

int16_t read_and_parse_sib(v8086* machine, uint8_t mod, uint16_t** segment, uint32_t *offset)
{
    uint8_t sib = read_byte_from_pointer(machine->Memory, get_absolute_address(machine->sregs.cs, machine->IP.w.ip + machine->internal_state.IPOffset));
    machine->internal_state.IPOffset += 1;
    uint8_t scale = sib >> 6u;
    uint8_t index = (uint8_t)(sib >> 3u) & 7u;
    uint8_t base = sib & 7u;

    switch (index) {
        case 0:
            *offset = machine->regs.d.eax;
            break;
        case 1:
            *offset = machine->regs.d.ecx;
            break;
        case 2:
            *offset = machine->regs.d.edx;
            break;
        case 3:
            *offset = machine->regs.d.ebx;
            break;
        case 4:
            *offset = 0;
            break;
        case 5:
            *offset = machine->regs.d.ebp;
            break;
        case 6:
            *offset = machine->regs.d.esi;
            break;
        case 7:
            *offset = machine->regs.d.edi;
            break;
        default:
            return V8086_BAD_INDEX;
    }
    *offset <<= scale;

    switch (base){
        case 0:
            *offset += machine->regs.d.eax;
            break;
        case 1:
            *offset += machine->regs.d.ecx;
            break;
        case 2:
            *offset += machine->regs.d.edx;
            break;
        case 3:
            *offset += machine->regs.d.ebx;
            break;
        case 4:
            *offset += machine->regs.d.esp;
            *segment = select_segment_register(machine, V8086_SS);
            break;
        case 5:
            if (mod == 0)
            {
                uint32_t disp = read_dword_from_pointer(machine->Memory, get_absolute_address(machine->sregs.cs,
                                                                                              machine->IP.w.ip +
                                                                                              machine->internal_state.IPOffset));
                machine->internal_state.IPOffset += 4;
                *offset += disp;
            }
            else
            {
                *offset += machine->regs.d.ebp;
                *segment = select_segment_register(machine, V8086_SS);
            }
            break;
        case 6:
            *offset += machine->regs.d.esi;
            break;
        case 7:
            *offset += machine->regs.d.edi;
            break;
        default:
            return V8086_BAD_BASE;
    }
    return V8086_OK;
}

int16_t calculate_segment_offset_from_mode_32(v8086 *machine, uint8_t mod_rm, uint16_t *segment, uint32_t *offset) {
    uint16_t* segment_register = NULL;
    uint8_t mod = mod_rm >> 6u; //Parsing mod than parsing rm
    uint8_t rm = mod_rm & 7u;

    switch(mod)
    {
        case 0:
            switch(rm)
            {
                case 0:
                    *offset = machine->regs.d.eax;
                    break;
                case 1:
                    *offset = machine->regs.d.ecx;
                    break;
                case 2:
                    *offset = machine->regs.d.edx;
                    break;
                case 3:
                    *offset = machine->regs.d.ebx;
                    break;
                case 4: // SIB
                    read_and_parse_sib(machine, mod, &segment_register, offset);
                    break;
                case 5:{
                    uint32_t disp = read_dword_from_pointer(machine->Memory, get_absolute_address(machine->sregs.cs,
                                                                                                  machine->IP.w.ip +
                                                                                                  machine->internal_state.IPOffset));
                    machine->internal_state.IPOffset += 4;
                    *offset = disp;
                    break;
                }
                case 6:
                    *offset = machine->regs.d.esi;
                    break;
                case 7:
                    *offset = machine->regs.d.edi;
                    break;
                default:
                    return V8086_BAD_RM;
            }
            break;
        case 1:
        {
            int8_t disp = read_byte_from_pointer(machine->Memory, get_absolute_address(machine->sregs.cs, machine->IP.w.ip + machine->internal_state.IPOffset));
            machine->internal_state.IPOffset += 1;
            switch (rm) {
                case 0:
                    *offset = machine->regs.d.eax + disp;
                    break;
                case 1:
                    *offset = machine->regs.d.ecx + disp;
                    break;
                case 2:
                    *offset = machine->regs.d.edx + disp;
                    break;
                case 3:
                    *offset = machine->regs.d.ebx + disp;
                    break;
                case 4: // SIB
                    machine->internal_state.IPOffset -= 1;
                    read_and_parse_sib(machine, mod, &segment_register, offset);
                    disp = read_byte_from_pointer(machine->Memory, get_absolute_address(machine->sregs.cs, machine->IP.w.ip + machine->internal_state.IPOffset));
                    machine->internal_state.IPOffset += 1;
                    *offset += disp;
                    break;
                case 5:{
                    *offset = machine->regs.d.ebp + disp;
                    segment_register = select_segment_register(machine, V8086_SS);
                    break;
                }
                case 6:
                    *offset = machine->regs.d.esi + disp;
                    break;
                case 7:
                    *offset = machine->regs.d.edi + disp;
                    break;
                default:
                    return V8086_BAD_RM;
            }
            break;
        }
        case 2: {
            uint32_t disp = read_dword_from_pointer(machine->Memory, get_absolute_address(machine->sregs.cs, machine->IP.w.ip + machine->internal_state.IPOffset));
            machine->internal_state.IPOffset += 4;
            switch (rm) {
                case 0:
                    *offset = machine->regs.d.eax + disp;
                    break;
                case 1:
                    *offset = machine->regs.d.ecx + disp;
                    break;
                case 2:
                    *offset = machine->regs.d.edx + disp;
                    break;
                case 3:
                    *offset = machine->regs.d.ebx + disp;
                    break;
                case 4: // SIB
                    machine->internal_state.IPOffset -= 4;
                    read_and_parse_sib(machine, mod, &segment_register, offset);
                    disp = read_dword_from_pointer(machine->Memory, get_absolute_address(machine->sregs.cs, machine->IP.w.ip + machine->internal_state.IPOffset));
                    machine->internal_state.IPOffset += 4;
                    *offset += disp;
                    break;
                case 5:{
                    *offset = machine->regs.d.ebp + disp;
                    segment_register = select_segment_register(machine, V8086_SS);
                    break;
                }
                case 6:
                    *offset = machine->regs.d.esi + disp;
                    break;
                case 7:
                    *offset = machine->regs.d.edi + disp;
                    break;
                default:
                    return V8086_BAD_RM;
            }
            break;
        }
        default:
            return V8086_BAD_MOD;
    }

    if(machine->internal_state.segment_reg_select != V8086_DEFAULT)
        segment_register = select_segment_register(machine, machine->internal_state.segment_reg_select);
    else if(segment_register == NULL)
        segment_register = select_segment_register(machine, V8086_DS);

    *segment = *segment_register;
    return 0;
}

void* get_memory_from_mode(v8086* machine, uint8_t mod_rm, uint8_t width)
{
    uint16_t segment;
    uint32_t offset;

    switch(mod_rm >> 6u) //Parsing mod than parsing rm
    {
        case 0:
        case 1:
        case 2:
            if(machine->internal_state.address_32_bit)
                calculate_segment_offset_from_mode_32(machine, mod_rm, &segment, &offset);
            else
                calculate_segment_offset_from_mode(machine, mod_rm, &segment, &offset);
            return get_variable_length_pointer(machine->Memory, get_absolute_address(segment, offset), width);
        case 3:
            switch(width){
                case 8:
                    return get_byte_register(machine, mod_rm & 7u);
                case 16:
                    return get_word_register(machine, mod_rm & 7u);
                case 32:
                    return get_dword_register(machine, mod_rm & 7u);
                default:
                    return NULL;
            }
    }
    return NULL;
}

