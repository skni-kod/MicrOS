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

void panic_screen_show(exception_state* state, uint32_t code, const char *optString)
{
    panic_screen_display_intro(state, code, optString);

    if (state != NULL)
    {
        vga_printstring("\n\nNacisnij dowolny klawisz aby przejsc do widoku diagnostycznego.");
        panic_screen_wait_for_key_press();
        panic_screen_display_diagnostic_view(state);
    }

    io_disable_interrupts();
    __asm__("hlt");
}

void panic_screen_display_intro(exception_state *state, uint32_t code, const char *optString)
{
    char buff[100];

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

void panic_screen_display_diagnostic_view(exception_state* state)
{
    vga_clear_screen();
    __asm__("hlt");
}