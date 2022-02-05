#include "panic_screen.h"
#include "drivers/dal/videocard/videocard.h"

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
    // if(vga_gmode_get_mode() != 0x3)
    //    set3Hvideo_mode();
    if(!video_card_is_text_mode())
        video_card_set_video_mode(0x3);
    vga_clear_screen();
    for (int i = 0; i < 20; i++)
    {
        vga_printstring(img[i]);
        vga_printchar('\n');
    }
    vga_printstring("Robimy to z bolem serca, ale musimy Ciebie o tym poinformowac... Zjebalo sie.\n");
    vga_printstring(panic_screen_value_to_string(buff, code));
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
    uint32_t valid_esp = panic_screen_is_privilege_level_changed(state) ? state->esp : state->registers.esp_unused + STACK_POINTER_OFFSET;

    vga_clear_screen();
    vga_printstring("Registers:\n\n");
    panic_screen_display_register_state("eax", state->registers.eax, true);
    panic_screen_display_register_state("ebx", state->registers.ebx, true);
    panic_screen_display_register_state("ecx", state->registers.ecx, true);
    panic_screen_display_register_state("edx", state->registers.edx, true);
    panic_screen_display_register_state("esi", state->registers.esi, true);
    panic_screen_display_register_state("edi", state->registers.edi, true);
    panic_screen_display_register_state("esp", valid_esp, true);
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

    panic_screen_display_stack(valid_esp, panic_screen_is_privilege_level_changed(state));
    panic_screen_display_system_clock(system_clock);

    vga_set_cursor_pos(45, 16);
    vga_printstring("FPU:");
    panic_screen_display_fpu_control_word(state->fpu_state.control_word);
    panic_screen_display_fpu_status_word(state->fpu_state.status_word);

    vga_set_cursor_pos(45, 20);
    panic_screen_display_register_state("opcode", state->fpu_state.opcode, true);

    vga_set_cursor_pos(0, 27);
    vga_printchar(' ');
    __asm__("hlt");
}

bool panic_screen_is_privilege_level_changed(exception_state *state)
{
    return state->ds == state->ss;
}

char *panic_screen_value_to_string(char *buffer, unsigned int value)
{
    unsigned int numbers[32];
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

void panic_screen_display_register_state(char *register_name, unsigned int value, bool new_line)
{
    char buffer[32];
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
    char *tags[] = {"CF", "", "PF", "", "AF", "", "ZF", "SF", "TF", "IF", "DF", "OF", "", "", "NT", "", "RF", "VM", "AC", "VIF", "VIP", "ID", 0};

    panic_screen_display_register_state("eflags", eflags, false);
    panic_screen_display_flag_tags(eflags, tags);
}

void panic_screen_display_cr0(uint32_t cr0)
{
    char *tags[] = {"PE", "MP", "EM", "TS", "ET", "NE", "", "", "", "", "", "", "", "", "", "", "WP", "", "AM", "", "", "", "", "", "", "", "", "", "", "NW", "CD", "PG", 0};

    panic_screen_display_register_state("cr0", cr0, false);
    panic_screen_display_flag_tags(cr0, tags);
}

void panic_screen_display_cr4(uint32_t cr4)
{
    char *tags[] = {"VME", "PVI", "TSD", "DE", "PSE", "PAE", "MCE", "PGE", "PCE", "OSFXSR", "OSXMMEXCPT", "UMIP", "", "VMXE", "SMXE", "", "", "PCIDE", "OSXSAVE", "", "SMEP", "SMAP", 0};

    panic_screen_display_register_state("cr4", cr4, false);
    panic_screen_display_flag_tags(cr4, tags);
}

void panic_screen_display_stack(uint32_t esp, bool user_stack)
{
    char buffer[16];
    uint32_t *addr = (uint32_t *)esp;

    vga_set_cursor_pos(45, 0);
    vga_printstring(user_stack ? "USER stack:\n" : "KERNEL stack:\n");

    for (int i = 2; i < 14; i++)
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
    char buffer[16];

    vga_set_cursor_pos(45, 22);
    vga_printstring("System clock: ");
    vga_printstring(itoa(system_clock, buffer, 10));
    vga_printstring(" ms");
}

void panic_screen_display_fpu_control_word(uint32_t control_word)
{
    char buffer[16];
    char *tags[] = {"IM", "DM", "ZM", "OM", "UM", "PM", 0};

    vga_set_cursor_pos(45, 18);
    vga_printstring("cw: 0x");
    vga_printstring(panic_screen_value_to_string(buffer, control_word));

    panic_screen_display_flag_tags(control_word, tags);
}

void panic_screen_display_fpu_status_word(uint32_t status_word)
{
    char buffer[16];
    char *tags[] = {"IE", "DE", "ZE", "OE", "UE", "PE", "SF", "ES", "C0", "C1", "C2", "", "", "", "C3", "B", 0};

    vga_set_cursor_pos(45, 19);
    vga_printstring("sw: 0x");

    vga_printstring(panic_screen_value_to_string(buffer, status_word));
    panic_screen_display_flag_tags(status_word, tags);
}

void panic_screen_display_flag_tags(uint32_t value, char **tags)
{
    vga_printstring(" [ ");

    int index = 0;
    while (tags[index] != 0)
    {
        if (value & (1 << index) && tags[index][0] != 0)
        {
            vga_printstring(tags[index]);
            vga_printstring(" ");
        }

        index++;
    }

    vga_printstring("]\n");
}