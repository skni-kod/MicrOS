#include "mod_rm_parsing.h"
#include <string.h>
#include "memory_operations.h"

void* get_variable_length_register(v8086* machine, uint8_t reg_field, uint8_t width)
{
    return width == 8 ? (void*)get_byte_register(machine, reg_field) : (void*)get_dword_register(machine, reg_field);
    /*switch (width)
    {
        case 8:
            return get_byte_register(machine, reg_field);
        case 16:
            return get_word_register(machine, reg_field);
        case 32:
            return get_dword_register(machine, reg_field);
        default:
            return NULL;
    }*/
}

int16_t calculate_segment_offset_from_mode(v8086* machine, uint8_t mod_rm, uint16_t* segment, uint32_t* offset)
{
    uint16_t* segment_register = NULL;
    if(machine->internal_state.segment_reg_select != V8086_DEFAULT)
        segment_register = select_segment_register(machine, machine->internal_state.segment_reg_select);
    else
    {
        /*if(((mod_rm & 7u) == 2) || ((mod_rm & 7u) == 3) || (((mod_rm & 7u) == 6) && ((mod_rm & 0xC0) == 0))){
            segment_register = select_segment_register(machine, V8086_SS);
        }
        else{
            segment_register = select_segment_register(machine, V8086_DS);
        }*/
        /*TODO: optimization screwed up on hardware*/
        /*TODO: consider better optimization*/
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

    if((mod_rm & 0xC0) < 0xC0)
    {
        if(machine->internal_state.address_32_bit)
            calculate_segment_offset_from_mode_32(machine, mod_rm, &segment, &offset);
        else
            calculate_segment_offset_from_mode(machine, mod_rm, &segment, &offset);
        return get_variable_length_pointer(machine->Memory, get_absolute_address(segment, offset), width);
    }
    else{
        return get_variable_length_register(machine, mod_rm & 7u, width);
    }
    return NULL;
}

