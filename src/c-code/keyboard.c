#include <keyboard.h>
#include "../include/stdio.h"
#include "../include/stdbool.h"

static unsigned char scancodes_table[128];

char keyboard_scancode_to_keycode(uint8_t scancode)
{
  if(scancode < 59)
    return scancodes_table[scancode];

  return 0;
}

static char curr_ch = 0;
static bool is_uppercase = false;

void keyboard_handler(uint8_t scancode)
{
  char ascii = keyboard_scancode_to_keycode(scancode);

  if(ascii == CAPS_LOCK)
  {
    is_uppercase = !is_uppercase;
    return;
  }

  if(is_uppercase && (ascii >= 97) && (ascii <= 122))
    putc(ascii - 32);
  
  else
    putc(ascii);

  curr_ch = ascii;
}

char keyboard_getch()
{
  if(curr_ch == '\n')
  {
    curr_ch = 0;
    return '\n';
  }

	return curr_ch;
}

// https://www.win.tue.nl/~aeb/linux/kbd/scancodes-1.html
static unsigned char scancodes_table[128] =
{
  0,  27, 
  '1', '2', '3', '4', '5', '6', '7', '8',	'9', '0', '_', '=', '\b',
  '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', 
  '\n',
  0,
  'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '"', 
  '~',   
  0,
  '\\', 
  'z', 'x', 'c', 'v', 'b', 'n', 'm', '<', '>', '/', 0,
  '*',
  0, ' ',
  CAPS_LOCK,
};
