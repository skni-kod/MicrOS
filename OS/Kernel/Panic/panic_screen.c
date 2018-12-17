#include "panic_screen.h"
#include "../Drivers/VGA/vga.h"
#include <stdlib.h>

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

void panic_screen_show(uint32_t code, const char *optString)
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

    __asm__("hlt");
}