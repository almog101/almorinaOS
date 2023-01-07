#include <keyboard.h>
#include "../include/stdio.h"
#include "../include/stdbool.h"

static unsigned char scancodes_table[128];
static unsigned char shift_symbols_table[59];

char keyboard_scancode_to_keycode(uint8_t scancode)
{
  if(scancode < 59)
    return scancodes_table[scancode];

  return 0;
}

static char curr_ch = 0;
static bool is_uppercase = false, is_shift = false;
static int line_len = 1;

void keyboard_handler(uint8_t scancode)
{
  char ascii = keyboard_scancode_to_keycode(scancode);
  //if((int)ascii != 0)
  //  printf("[%d]", (int)ascii);

  if(ascii == CAPS_LOCK)
  {
    is_uppercase = !is_uppercase;
    return;
  }

  if(ascii == LEFT_SHIFT || ascii == RIGHT_SHIFT)
  {
    is_shift = true;
    return;
  }

  if(ascii == '\n')
    line_len = 1;

  if(ascii != '\n' && ascii != '\b' && (int)ascii != 0)
    line_len++;

  if((is_uppercase || is_shift) && (ascii >= 'a') && (ascii <= 'z'))
  {
    ascii -= 32;
    is_shift = false;
  }

  if(is_shift && !((ascii >= 'a') && (ascii <= 'z')))
  {
    ascii = shift_symbols_table[ascii - '\''];
    is_shift = false;
  }

  if(ascii == '\b')
  {
    line_len--;
    if(line_len < 0)
      line_len = 0;
  }

  if(line_len > 0)
    putc(ascii);

  curr_ch = ascii;
}

char keyboard_getch()
{
  char temp = curr_ch;
  curr_ch = 0;
	return temp;
}

// https://www.win.tue.nl/~aeb/linux/kbd/scancodes-1.html
static unsigned char scancodes_table[128] =
{
  0,  27, 
  '1', '2', '3', '4', '5', '6', '7', '8',	'9', '0', '-', '=', '\b',
  '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', 
  '\n',
  0,
  'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', 
  '`',
  LEFT_SHIFT,
  '\\', 
  'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', RIGHT_SHIFT,
  '*',
  0, ' ',
  CAPS_LOCK,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0,
  0,
  0, 0, 0,
  0,
  0, 0, 0, 0,
  0, 0, 0,
  0, 0
};

static unsigned char shift_symbols_table[59] =
{
  '"', 0, 0, 0, 0, '<', '_', '>', '?',
  /* 0 - 9 */
  ')', '!', '@', '#', '$', '%', '^', '&', '*', '(',
  0, ':', 0, '+', 0, 0,
  /* A - Z */
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  '{', '{', '|', '}', 0, 0, '~'
};
