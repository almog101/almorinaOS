#ifndef KEYCODE_H
#define KEYCODE_H
#include <stdint.h>

#define BACKSPACE_CHAR 0x08
#define TAB_CHAR 0x09
#define RETURN_CHAR '\r'
#define NEWLINE_CHAR '\n'
#define BLANK_CHAR ' '

#define CAPS_LOCK 58

void keyboard_handler(uint8_t scancode);
char keyboard_getch();

#endif
