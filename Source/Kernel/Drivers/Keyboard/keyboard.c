#include "keyboard.h"
#include "../../Drivers/VGA/vga.h"

unsigned char kbdus[128] =
{
    0,  27, '1', '2', '3', '4', '5', '6', '7', '8',	/* 9 */
  '9', '0', '-', '=', '\b',	/* Backspace */
  '\t',			/* Tab */
  'q', 'w', 'e', 'r',	/* 19 */
  't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',	/* Enter key */
    0,			/* 29   - Control */
  'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';',	/* 39 */
 '\'', '`',   0,		/* Left shift */
 '\\', 'z', 'x', 'c', 'v', 'b', 'n',			/* 49 */
  'm', ',', '.', '/',   0,				/* Right shift */
  '*',
    0,	/* Alt */
  ' ',	/* Space bar */
    0,	/* Caps lock */
    0,	/* 59 - F1 key ... > */
    0,   0,   0,   0,   0,   0,   0,   0,
    0,	/* < ... F10 */
    0,	/* 69 - Num lock*/
    0,	/* Scroll Lock */
    0,	/* Home key */
    0,	/* Up Arrow */
    0,	/* Page Up */
  '-',
    0,	/* Left Arrow */
    0,
    0,	/* Right Arrow */
  '+',
    0,	/* 79 - End key*/
    0,	/* Down Arrow */
    0,	/* Page Down */
    0,	/* Insert Key */
    0,	/* Delete Key */
    0,   0,   0,
    0,	/* F11 Key */
    0,	/* F12 Key */
    0,	/* All other keys are undefined */
};	

unsigned char kbdusB[128] =
{
    0,  27, '!', '@', '#', '$', '%', '^', '&', '*',	/* 9 */
  '(', ')', '_', '+', '\b',	/* Backspace */
  '\t',			/* Tab */
  'Q', 'W', 'E', 'R',	/* 19 */
  'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', '\n',	/* Enter key */
    0,			/* 29   - Control */
  'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':',	/* 39 */
 '\"', '~',   0,		/* Left shift */
 '|', 'Z', 'X', 'C', 'V', 'B', 'N',			/* 49 */
  'M', '<', '>', '?',   0,				/* Right shift */
  '*',
    0,	/* Alt */
  ' ',	/* Space bar */
    0,	/* Caps lock */
    0,	/* 59 - F1 key ... > */
    0,   0,   0,   0,   0,   0,   0,   0,
    0,	/* < ... F10 */
    0,	/* 69 - Num lock*/
    0,	/* Scroll Lock */
    0,	/* Home key */
    0,	/* Up Arrow */
    0,	/* Page Up */
  '-',
    0,	/* Left Arrow */
    0,
    0,	/* Right Arrow */
  '+',
    0,	/* 79 - End key*/
    0,	/* Down Arrow */
    0,	/* Page Down */
    0,	/* Insert Key */
    0,	/* Delete Key */
    0,   0,   0,
    0,	/* F11 Key */
    0,	/* F12 Key */
    0,	/* All other keys are undefined */
};	

volatile KeyboardStateFlags* kb_state = 0x0417;
volatile KeyboardExtendedStateFlags* kb_estate = 0x0496;

unsigned char get_scancode()
{
    return inb(0x60);
}

void keyboard_handler()
{ 
     unsigned char scancode; 
     unsigned int shift_key = 0;
     scancode = get_scancode();
     //scancode = d;

    if(scancode == 0xE0)
        kb_estate->last_E0h = 1;
    else if(scancode == 0xE1)
        kb_estate->last_E1h = 1;
    else
    {
      switch(scancode){
        case 29:
          kb_state->ctrl_pressed = 1;
          if(kb_estate->last_E0h)    
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
          if(kb_estate->last_E0h)    
            kb_estate->right_alt_pressed = 1;
          else
            kb_state->left_alt_pressed = 1;
          break;
        case 29 + 128:
          kb_state->ctrl_pressed = 0;
          if(kb_estate->last_E0h)    
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
          if(kb_estate->last_E0h)    
            kb_estate->right_alt_pressed = 0;
          else
            kb_state->left_alt_pressed = 0;
          break;
        default:
          if(scancode < 128)
          {
            if((kb_state->left_shift_pressed) || (kb_state->right_shift_pressed))
              vga_printchar(kbdusB[scancode]);
            else
              vga_printchar(kbdus[scancode]);
          }
      }
      kb_estate->last_E0h = 0;
      kb_estate->last_E1h = 0;
    }
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