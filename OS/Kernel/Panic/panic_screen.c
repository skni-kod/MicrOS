#include "panic_screen.h"

const char *img[] =
    {
        (const char *)". . . . . .. . . . . . . . . . . ,.-'\". . . . . . . . . .\"~.,",
        (const char *)". . . . . . . .. . . . . .,.-\". . . . . . . . . . . . . . . . . .\"-.,",
        (const char *)". . . . .. . . . . . ..,/. . . . . . . . . . . . . . . . . . . . . . . \":,",
        (const char *)". . . . . . . .. .,?. . . . . . . . . . . . . . . . . . . . . . . . . . .\\,",
        (const char *)". . . . . . . . . /. . . . . . . . . . . . . . . . . . . . . . . . . . . . ,}",
        (const char *)". . . . . . . . ./. . . . . . . . . . . . . . . . . . . . . . . . . . ,:`^`.}",
        (const char *)". . . . . . . ./. . . . . . . . . . . . . . . . . . . . . . . . . ,:\". . . ./",
        (const char *)". . . . . . .?. . . __. . . . . . . . . . . . . . . . . . . . :`. . . ./",
        (const char *)". . . . . . . /__.(. . .\"~-,_. . . . . . . . . . . . . . ,:`. . . .. ./",
        (const char *)". . . . . . /(_. . \"~,_. . . ..\"~,_. . . . . . . . . .,:`. . . . _/",
        (const char *)". . . .. .{.._$;_. . .\"=,_. . . .\"-,_. . . ,.-~-,}, .~\"; /. .. .}",
        (const char *)". . .. . .((. . .*~_. . . .\"=-._. . .\";,,./`. . /\" . . . ./. .. ../",
        (const char *)". . . .. . .\\`~,. . ..\"~.,. . . . . . . . . ..`. . .}. . . . . . ../",
        (const char *)". . . . . .(. ..`=-,,. . . .`. . . . . . . . . . . ..(. . . ;_,,-\"",
        (const char *)". . . . . ../.`~,. . ..`-.. . . . . . . . . . . . . . ..\\. . /\\",
        (const char *)". . . . . . \\`~.*-,. . . . . . . . . . . . . . . . . ..|,./...\\,__",
        (const char *)",,_. . . . . }.>-._\\. . . . . . . . . . . . . . . . . .|. . . . . . ..`=~-,",
        (const char *)". .. `=~-,_\\_. . . `\\,. . . . . . . . . . . . . . . . .\\",
        (const char *)". . . . . . . . . .`=~-,,.\\,. . . . . . . . . . . . . . . .\\",
        (const char *)". . . . . . . . . . . . . . . . `:,, . . . . . . . . . . . . . `\\. . . . . .",
};

void panic_screen_show(exception_state *state, uint32_t code, const char *optString)
{
    uint32_t system_clock = timer_get_system_clock();
    panic_screen_display_intro(state, code, optString);

    if (state != NULL)
    {
        vga_printstring("\n\nNacisnij dowolny klawisz aby przejsc do widoku diagnostycznego.");
        panic_screen_wait_for_key_press();
        panic_screen_display_diagnostic_view(state, system_clock);
    }

    io_disable_interrupts();
    __asm__("hlt");
}

void panic_screen_display_intro(exception_state *state, uint32_t code, const char *optString)
{
    char buff[100];
    if(getMode() != 0x3)
        set3HVideoMode();
    vga_clear_screen();
    for (int i = 0; i < 20; i++)
    {
        vga_printstring(img[i]);
        vga_printchar('\n');
    }
    vga_printstring("Robimy to z bolem serca, ale musimy Ciebie o tym poinformowac... Zjebalo sie.\n");
    vga_printstring("0x");
    vga_printstring(itoa(code, buff, 16));
    vga_printchar(' ');
    if (optString != 0x0)
    {
        vga_printstring(optString);
    }
}

void panic_screen_wait_for_key_press()
{
    while (1)
    {
        if (!keyboard_is_buffer_empty())
            break;
        __asm__("hlt");
    }
}
void panic_screen_display_diagnostic_view(exception_state *state, uint32_t system_clock)
{
    vga_clear_screen();
    vga_printstring("Registers:\n\n");
    panic_screen_display_register_state("eax", state->registers.eax, true);
    panic_screen_display_register_state("ebx", state->registers.ebx, true);
    panic_screen_display_register_state("ecx", state->registers.ecx, true);
    panic_screen_display_register_state("edx", state->registers.edx, true);
    panic_screen_display_register_state("esi", state->registers.esi, true);
    panic_screen_display_register_state("edi", state->registers.edi, true);
    panic_screen_display_register_state("esp", state->registers.esp_unused + STACK_POINTER_OFFSET, true);
    panic_screen_display_register_state("eip", state->eip, true);
    vga_printstring("\n");

    panic_screen_display_register_state("cs", state->cs, true);
    panic_screen_display_register_state("ss", state->ss, true);
    panic_screen_display_register_state("ds", state->ds, true);
    vga_printstring("\n");

    panic_screen_display_cr0(state->cr0);
    panic_screen_display_register_state("cr2", state->cr2, true);
    panic_screen_display_register_state("cr3", state->cr3, true);
    panic_screen_display_cr4(state->cr4);
    panic_screen_display_eflags(state->eflags);
    vga_printstring("\n");

    panic_screen_display_descriptor_table("gdtr", state->gdtr, sizeof(gdt_entry));
    panic_screen_display_descriptor_table("idtr", state->idtr, sizeof(idt_entry));

    panic_screen_display_stack(state->registers.esp_unused + STACK_POINTER_OFFSET);
    panic_screen_display_system_clock(system_clock);

    vga_set_cursor_pos(45, 17);
    vga_printstring("FPU:");
    panic_screen_display_fpu_control_word(state->fpu_state.control_word);
    panic_screen_display_fpu_status_word(state->fpu_state.status_word);

    vga_set_cursor_pos(0, 27);
    vga_printchar(' ');
    __asm__("hlt");
}

char *panic_screen_value_to_string(char *buffer, unsigned int value)
{
    unsigned int numbers[32] = {0};
    int length = 0;

    while (value != 0)
    {
        numbers[length] = value % 16;
        value /= 16;
        length++;
    }

    for (int i = length - 1; i >= 0; i--)
    {
        buffer[length - 1 - i] = numbers[i] >= 10 ? numbers[i] - 10 + 'a' : numbers[i] + '0';
    }

    if (length == 0)
    {
        buffer[0] = '0';
        length++;
    }

    buffer[length] = 0;

    return buffer;
}

void panic_screen_display_register_state(char *register_name, int value, bool new_line)
{
    char buffer[32] = {0};
    vga_printstring(register_name);
    vga_printstring(": 0x");
    vga_printstring(panic_screen_value_to_string(buffer, value));

    if (new_line)
    {
        vga_printstring("\n");
    }
}

void panic_screen_display_eflags(uint32_t eflags)
{
    panic_screen_display_register_state("eflags", eflags, false);

    vga_printstring(" [");
    if (eflags & (1 << 0))
        vga_printstring(" CF");
    if (eflags & (1 << 2))
        vga_printstring(" PF");
    if (eflags & (1 << 4))
        vga_printstring(" AF");
    if (eflags & (1 << 6))
        vga_printstring(" ZF");
    if (eflags & (1 << 7))
        vga_printstring(" SF");
    if (eflags & (1 << 8))
        vga_printstring(" TF");
    if (eflags & (1 << 9))
        vga_printstring(" IF");
    if (eflags & (1 << 10))
        vga_printstring(" DF");
    if (eflags & (1 << 11))
        vga_printstring(" OF");
    if (eflags & (1 << 14))
        vga_printstring(" NT");
    if (eflags & (1 << 16))
        vga_printstring(" RF");
    if (eflags & (1 << 17))
        vga_printstring(" VM");
    if (eflags & (1 << 18))
        vga_printstring(" AC");
    if (eflags & (1 << 19))
        vga_printstring(" VIF");
    if (eflags & (1 << 20))
        vga_printstring(" VIP");
    if (eflags & (1 << 21))
        vga_printstring(" ID");
    vga_printstring(" ]\n");
}

void panic_screen_display_cr0(uint32_t cr0)
{
    panic_screen_display_register_state("cr0", cr0, false);

    vga_printstring(" [");
    if (cr0 & (1 << 0))
        vga_printstring(" PE");
    if (cr0 & (1 << 1))
        vga_printstring(" MP");
    if (cr0 & (1 << 2))
        vga_printstring(" EM");
    if (cr0 & (1 << 3))
        vga_printstring(" TS");
    if (cr0 & (1 << 4))
        vga_printstring(" ET");
    if (cr0 & (1 << 5))
        vga_printstring(" NE");
    if (cr0 & (1 << 16))
        vga_printstring(" WP");
    if (cr0 & (1 << 18))
        vga_printstring(" AM");
    if (cr0 & (1 << 29))
        vga_printstring(" NW");
    if (cr0 & (1 << 30))
        vga_printstring(" CD");
    if (cr0 & (1 << 31))
        vga_printstring(" PG");
    vga_printstring(" ]\n");
}

void panic_screen_display_cr4(uint32_t cr4)
{
    panic_screen_display_register_state("cr4", cr4, false);

    vga_printstring(" [");
    if (cr4 & (1 << 0))
        vga_printstring(" VME");
    if (cr4 & (1 << 1))
        vga_printstring(" PVI");
    if (cr4 & (1 << 2))
        vga_printstring(" TSD");
    if (cr4 & (1 << 3))
        vga_printstring(" DE");
    if (cr4 & (1 << 4))
        vga_printstring(" PSE");
    if (cr4 & (1 << 5))
        vga_printstring(" PAE");
    if (cr4 & (1 << 6))
        vga_printstring(" MCE");
    if (cr4 & (1 << 7))
        vga_printstring(" PGE");
    if (cr4 & (1 << 8))
        vga_printstring(" PCE");
    if (cr4 & (1 << 9))
        vga_printstring(" OSFXSR");
    if (cr4 & (1 << 10))
        vga_printstring(" OSXMMEXCPT");
    if (cr4 & (1 << 11))
        vga_printstring(" UMIP");
    if (cr4 & (1 << 13))
        vga_printstring(" VMXE");
    if (cr4 & (1 << 14))
        vga_printstring(" SMXE");
    if (cr4 & (1 << 17))
        vga_printstring(" PCIDE");
    if (cr4 & (1 << 18))
        vga_printstring(" OSXSAVE");
    if (cr4 & (1 << 20))
        vga_printstring(" SMEP");
    if (cr4 & (1 << 21))
        vga_printstring(" SMAP");
    vga_printstring(" ]\n");
}

void panic_screen_display_stack(uint32_t esp)
{
    char buffer[16] = {0};
    uint32_t *addr = (uint32_t *)esp;

    vga_set_cursor_pos(45, 0);
    vga_printstring("Stack:\n");

    for (int i = 2; i < 15; i++)
    {
        vga_set_cursor_pos(45, i);

        panic_screen_value_to_string(buffer, (uint32_t)addr);
        vga_printstring("0x");
        vga_printstring(buffer);
        vga_printstring(" = ");

        panic_screen_value_to_string(buffer, *addr);
        vga_printstring("0x");
        vga_printstring(buffer);
        vga_printstring("\n");

        addr++;
    }
}

void panic_screen_display_descriptor_table(char *name, uint64_t value, uint32_t size_of_entry)
{
    char buffer[16];

    uint32_t size = value & 0xFFFF;
    uint32_t base = value >> 16;

    uint32_t entries_count = size / size_of_entry + 1;

    panic_screen_display_register_state(name, value, false);
    vga_printstring(" (c: ");
    vga_printstring(panic_screen_value_to_string(buffer, entries_count));
    vga_printstring(", b: 0x");
    vga_printstring(panic_screen_value_to_string(buffer, base));
    vga_printstring(")\n");
}

void panic_screen_display_system_clock(uint32_t system_clock)
{
    char buffer[16] = {0};

    vga_set_cursor_pos(45, 22);
    vga_printstring("System clock: ");
    vga_printstring(itoa(system_clock, buffer, 10));
    vga_printstring(" ms");
}

void panic_screen_display_fpu_control_word(uint32_t control_word)
{
    char buffer[16] = {0};

    vga_set_cursor_pos(45, 19);
    vga_printstring("cw: 0x");
    vga_printstring(itoa(control_word, buffer, 10));

    vga_printstring(" [");
    if (control_word & (1 << 0))
        vga_printstring(" IM");
    if (control_word & (1 << 1))
        vga_printstring(" DM");
    if (control_word & (1 << 2))
        vga_printstring(" ZM");
    if (control_word & (1 << 3))
        vga_printstring(" OM");
    if (control_word & (1 << 4))
        vga_printstring(" UM");
    if (control_word & (1 << 5))
        vga_printstring(" PM");
    vga_printstring(" ]\n");
}

void panic_screen_display_fpu_status_word(uint32_t status_word)
{
    char buffer[16] = {0};

    vga_set_cursor_pos(45, 20);
    vga_printstring("sw: 0x");
    vga_printstring(itoa(status_word, buffer, 10));

    vga_printstring(" [");
    if (status_word & (1 << 0))
        vga_printstring(" IE");
    if (status_word & (1 << 1))
        vga_printstring(" DE");
    if (status_word & (1 << 2))
        vga_printstring(" ZE");
    if (status_word & (1 << 3))
        vga_printstring(" OE");
    if (status_word & (1 << 4))
        vga_printstring(" UE");
    if (status_word & (1 << 5))
        vga_printstring(" PE");
    if (status_word & (1 << 6))
        vga_printstring(" SF");
    if (status_word & (1 << 7))
        vga_printstring(" ES");
    vga_printstring(" ]\n");
}