#include "tests.h"

#ifdef TEST_V8086
#include "stdint.h"
#include "../mod_rm_parsing.h"
#include "../drivers/serial/serial.h"
#include "string.h"
#include "../../debug_helpers/library/kernel_stdio.h"

char* effective_addresses[3][8] = {
    {"[BX+SI]", "[BX+DI]", "[BP+SI]", "[BP+DI]", "[SI]", "[DI]", "disp16", "[BX]"},
    {"[BX+SI]+disp8", "[BX+DI]+disp8", "[BP+SI]+disp8", "[BP+DI]+disp8", "[SI]+disp8", "[DI]+disp8", "[BP]+disp8", "[BX]+disp8"},
    {"[BX+SI]+disp16", "[BX+DI]+disp16", "[BP+SI]+disp16", "[BP+DI]+disp16", "[SI]+disp16", "[DI]+disp16", "[BP]+disp16", "[BX]+disp16"}
};

#define test_mod_16_func(width) void test_mod_16_width_##width(v8086* machine){\
    uint##width##_t* memory_##width;\
    uint##width##_t* expected_memory_##width;\
    machine->IP.w.ip = 0; \
    machine->sregs.cs = 0; \
    machine->regs.x.bx = 0; \
    machine->regs.x.bp = 0; \
    machine->regs.x.si = 0; \
    machine->regs.x.di = 0; \
    char str[100] = ""; \
    for(uint16_t mod_rm=0; mod_rm < 192; mod_rm++) \
    { \
        char* effective_address = effective_addresses[mod_rm >> 6][mod_rm & 7]; \
        uint16_t cumulative_offset = 0; \
        uint8_t bp_on_road = 0; \
        if(strstr(effective_address, "BX") != NULL) \
        { \
            uint16_t offset = 98; \
            machine->regs.x.bx = offset; \
            cumulative_offset += offset; \
        } \
        if(strstr(effective_address, "BP") != NULL) \
        { \
            uint16_t offset = 56; \
            machine->regs.x.bp = offset; \
            cumulative_offset += offset; \
            bp_on_road = 1; \
        } \
        if(strstr(effective_address, "SI") != NULL) \
        { \
            uint16_t offset = 15; \
            machine->regs.x.si = offset; \
            cumulative_offset += offset; \
        } \
        if(strstr(effective_address, "DI") != NULL) \
        { \
            uint16_t offset = 32; \
            machine->regs.x.di = offset; \
            cumulative_offset += offset; \
        } \
        if(strstr(effective_address, "disp8") != NULL) \
        { \
            int16_t offset = 69; \
            machine->Memory[0] = offset; \
            cumulative_offset += offset; \
        } \
        if(strstr(effective_address, "disp16") != NULL) \
        { \
            uint16_t offset = 1520; \
            machine->Memory[0] = offset & 0xff; \
            machine->Memory[1] = offset >> 8; \
            cumulative_offset += offset; \
        } \
        memory_##width = (uint##width##_t*)get_memory_from_mode(machine, mod_rm, width); \
        expected_memory_##width = (uint##width##_t*)(machine->Memory + cumulative_offset + (bp_on_road ? machine->sregs.ss:machine->sregs.ds) * 0x10);\
        machine->internal_state.IPOffset = 0; \
        machine->regs.x.bx = 0; \
        machine->regs.x.bp = 0; \
        machine->regs.x.si = 0; \
        machine->regs.x.di = 0; \
        if(memory_##width != expected_memory_##width){ \
            kernel_sprintf(str, "ERROR for mode %x and width %d\n", mod_rm, width); \
            serial_send_string(COM1_PORT, str); \
        } \
        else{ \
            kernel_sprintf(str, "OK for mode %x and width %d\n", mod_rm, width); \
            serial_send_string(COM1_PORT, str); \
        } \
    } \
}

test_mod_16_func(8)
test_mod_16_func(16)
test_mod_16_func(32)

void test_mod_16()
{
    v8086* machine = v8086_create_machine();

    test_mod_16_width_8(machine);
    test_mod_16_width_16(machine);
    test_mod_16_width_32(machine);

    v8086_destroy_machine(machine);
}

#endif