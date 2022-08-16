#include "keyboard.h"
#include "drivers/vga/vga.h"

unsigned char kbdus[128] =
    {
        0, 27, '1', '2', '3', '4', '5', '6', '7', '8',    /* 9 */
        '9', '0', '-', '=', '\b',                         /* Backspace */
        '\t',                                             /* Tab */
        'q', 'w', 'e', 'r',                               /* 19 */
        't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',     /* Enter key */
        0,                                                /* 29   - Control */
        'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', /* 39 */
        '\'', '`', 0,                                     /* Left shift */
        '\\', 'z', 'x', 'c', 'v', 'b', 'n',               /* 49 */
        'm', ',', '.', '/', 0,                            /* Right shift */
        '*',
        0,   /* Alt */
        ' ', /* Space bar */
        0,   /* Caps lock */
        0,   /* 59 - F1 key ... > */
        0, 0, 0, 0, 0, 0, 0, 0,
        0, /* < ... F10 */
        0, /* 69 - Num lock*/
        0, /* Scroll Lock */
        0, /* Home key */
        0, /* Up Arrow */
        0, /* Page Up */
        '-',
        0, /* Left Arrow */
        0,
        0, /* Right Arrow */
        '+',
        0, /* 79 - End key*/
        0, /* Down Arrow */
        0, /* Page Down */
        0, /* Insert Key */
        0, /* Delete Key */
        0, 0, 0,
        0, /* F11 Key */
        0, /* F12 Key */
        0, /* All other keys are undefined */
};

unsigned char kbdusB[128] =
    {
        0, 27, '!', '@', '#', '$', '%', '^', '&', '*',    /* 9 */
        '(', ')', '_', '+', '\b',                         /* Backspace */
        '\t',                                             /* Tab */
        'Q', 'W', 'E', 'R',                               /* 19 */
        'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', '\n',     /* Enter key */
        0,                                                /* 29   - Control */
        'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', /* 39 */
        '\"', '~', 0,                                     /* Left shift */
        '|', 'Z', 'X', 'C', 'V', 'B', 'N',                /* 49 */
        'M', '<', '>', '?', 0,                            /* Right shift */
        '*',
        0,   /* Alt */
        ' ', /* Space bar */
        0,   /* Caps lock */
        0,   /* 59 - F1 key ... > */
        0, 0, 0, 0, 0, 0, 0, 0,
        0,   /* < ... F10 */
        0,   /* 69 - Num lock*/
        0,   /* Scroll Lock */
        '7', /* Home key */
        '8', /* Up Arrow */
        '9', /* Page Up */
        '-',
        '4', /* Left Arrow */
        '5',
        '6', /* Right Arrow */
        '+',
        '1', /* 79 - End key*/
        '2', /* Down Arrow */
        '3', /* Page Down */
        '0', /* Insert Key */
        ',', /* Delete Key */
        0, 0, 0,
        0, /* F11 Key */
        0, /* F12 Key */
        0, /* All other keys are undefined */
};

bool keyboard_keys_state[128];

volatile keyboard_state_flags *const kb_state = (keyboard_state_flags *)0xc0000417;
volatile keyboard_extended_state_flags *const kb_estate = (keyboard_extended_state_flags *)0xc0000496;
volatile uint16_t *const buffer_start = (uint16_t *)0xc0000480;
volatile uint16_t *const buffer_end = (uint16_t *)0xc0000482;
volatile uint16_t *const buffer_write = (uint16_t *)0xc000041C;
volatile uint16_t *const buffer_read = (uint16_t *)0xc000041A;

void keyboard_init()
{
    pic_enable_irq(1);
    idt_attach_interrupt_handler(1, keyboard_handler);
}

unsigned char keyboard_able_to_write()
{
    if (((*buffer_end) == (*buffer_write)) && ((*buffer_start) == (*buffer_read)))
        return 0;
    if ((*buffer_write) + 2 == (*buffer_read))
        return 0;
    return 1;
}

unsigned char keyboard_is_buffer_empty()
{
    return (*buffer_write) == (*buffer_read);
}

bool keyboard_get_key_state(char scancode)
{
    return keyboard_keys_state[(int)scancode];
}

void keyboard_increment_buffer_pointer(volatile uint16_t *const ptr)
{
    if (*ptr == (*buffer_end))
        *ptr = (*buffer_start);
    else
        *ptr = *ptr + 2;
}

void keyboard_put_key_to_buffer(unsigned char scancode, unsigned char ascii)
{
    if (!keyboard_able_to_write())
    {
        vga_printstring("\n _._     _,-'\"\"`-._       YEAH OKAY WE GET IT CRUMPY CAT IS FUNNY LOOKING\n");
        vga_printstring("(,-.`._,'(       |\\`-/|                                                  \n");
        vga_printstring("    `-.-' \\ )-`( , o o)                                                  \n");
        vga_printstring("-bf-      `-    \\`_`\"'-   WE FUCKING GET IT ALREADY. WE PLUG THE BUFFER  \n");
        return;
    }

    volatile keyboard_scan_ascii_pair *character = (keyboard_scan_ascii_pair *)((0xc0000000 + (0x0040 * 0x10)) + (*buffer_write));
    character->ascii = ascii;
    character->scancode = scancode;
    keyboard_increment_buffer_pointer(buffer_write);
}

bool keyboard_get_key_from_buffer(keyboard_scan_ascii_pair *scan_ascii_pair)
{
    if (keyboard_is_buffer_empty())
    {
        return false;
    }

    keyboard_scan_ascii_pair *tmp = (keyboard_scan_ascii_pair *)((0xc0000000 + (0x0040 * 0x10)) + (*buffer_read));
    scan_ascii_pair->ascii = tmp->ascii;
    scan_ascii_pair->scancode = tmp->scancode;
    keyboard_increment_buffer_pointer(buffer_read);

    return true;
}

unsigned char keyboard_get_scancode()
{
    return io_in_byte(0x60);
}

bool keyboard_handler()
{
    unsigned char scancode;
    scancode = keyboard_get_scancode();
    //scancode = d;

    if (scancode == 0xE0)
        kb_estate->last_E0h = 1;
    else if (scancode == 0xE1)
        kb_estate->last_E1h = 1;
    else
    {
        switch (scancode)
        {
        case 29:
            kb_state->ctrl_pressed = 1;
            if (kb_estate->last_E0h)
                kb_estate->right_ctrl_pressed = 1;
            else
                kb_state->left_ctrl_pressed = 1;
            break;
        case 42:
            kb_state->left_shift_pressed = 1;
            break;
        case 54:
            kb_state->right_shift_pressed = 1;
            break;
        case 56:
            kb_state->alt_pressed = 1;
            if (kb_estate->last_E0h)
                kb_estate->right_alt_pressed = 1;
            else
                kb_state->left_alt_pressed = 1;
            break;
        case 58:
            kb_state->caps_lock_pressed = 1;
            kb_state->caps_lock_active = !kb_state->caps_lock_active;
            kb_estate->caps_lock_led = !kb_estate->caps_lock_led;
            break;
        case 69:
            kb_state->num_lock_pressed = 1;
            kb_state->num_lock_active = !kb_state->num_lock_active;
            kb_estate->num_lock_led = !kb_estate->num_lock_led;
            break;
        case 70:
            kb_state->scroll_lock_pressed = 1;
            kb_state->scroll_lock_active = !kb_state->scroll_lock_active;
            kb_estate->scroll_lock_led = !kb_estate->scroll_lock_led;
            break;
        case 29 + 128:
            kb_state->ctrl_pressed = 0;
            if (kb_estate->last_E0h)
                kb_estate->right_ctrl_pressed = 0;
            else
                kb_state->left_ctrl_pressed = 0;
            break;
        case 42 + 128:
            kb_state->left_shift_pressed = 0;
            break;
        case 54 + 128:
            kb_state->right_shift_pressed = 0;
            break;
        case 56 + 128:
            kb_state->alt_pressed = 0;
            if (kb_estate->last_E0h)
                kb_estate->right_alt_pressed = 0;
            else
                kb_state->left_alt_pressed = 0;
            break;
        case 58 + 128:
            kb_state->caps_lock_pressed = 0;
            break;
        case 69 + 128:
            kb_state->num_lock_pressed = 0;
            break;
        case 70 + 128:
            kb_state->scroll_lock_pressed = 0;
            break;
        default:
            if (scancode < 128)
            {
                if ((scancode >= 71) && (scancode <= 83))
                {
                    if (kb_state->num_lock_active)
                        keyboard_put_key_to_buffer(scancode, kbdusB[scancode]);
                    else
                        keyboard_put_key_to_buffer(scancode, kbdus[scancode]);
                }
                else if (kb_state->caps_lock_active)
                {
                    if ((kb_state->left_shift_pressed) || (kb_state->right_shift_pressed))
                        keyboard_put_key_to_buffer(scancode, kbdus[scancode]);
                    else
                        keyboard_put_key_to_buffer(scancode, kbdusB[scancode]);
                }
                else
                {
                    if ((kb_state->left_shift_pressed) || (kb_state->right_shift_pressed))
                        keyboard_put_key_to_buffer(scancode, kbdusB[scancode]);
                    else
                        keyboard_put_key_to_buffer(scancode, kbdus[scancode]);
                }

                keyboard_keys_state[scancode] = true;
            }
            else
            {
                int released_scan_code = scancode - 128;
                keyboard_keys_state[released_scan_code] = false;
            }
        }
        kb_estate->last_E0h = 0;
        kb_estate->last_E1h = 0;
    }
    
    return false;

    // TODO: Check if these commented lines are necessary.
    /*if(scancode == 0xE0)
    {
      kb_estate->last_E0h = 1;
    }
    else if(scancode == 0xE1)
    {
      kb_estate->last_E0h = 1;
    }
    else if(scancode > 128)
    {

    }
    else{
      if(scancode == 29)
      {

      }
    }*/
    /*if(scancode == 0x2A)     
     {
          shift_key = 1;//Shift key is pressed
     }      
     else if(scancode == 0xAA)
     {
          int shift_key= 0;//Shift Key is not pressed
     }      
     else    
     {          
         if (scancode == 0x80)
         {
              int shiftaltctrl = 1;//Put anything to see what special keys were pressed
         }
         else
         {  
              vga_printchar(kbdus[scancode]); //Prints the character which was pressed         
         }     
     }*/
}

unsigned char keyboard_is_between(unsigned char number, unsigned char l, unsigned char r)
{
    return (number > l) && (number < r);
}

unsigned char keyboard_is_between_or_eq(unsigned char number, unsigned char l, unsigned char r)
{
    return (number >= l) && (number <= r);
}

keyboard_state_flags *keyboard_get_state_flags()
{
    return (keyboard_state_flags *) kb_state;
}