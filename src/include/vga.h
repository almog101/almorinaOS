#ifndef VGA_H
#define VGA_H

#include <stdint.h>
#define SCREEN_WIDTH 80
#define SCREEN_HEIGT 25

enum Color16
{
	BLACK,
    BLUE,
    GREEN,
    CYAN,
    RED,
    MAGENTA,
    BROWN,
    LIGHTGREY,
    DARKGREY,
    LIGHTBLUE,
    LIGHTGREEN,
    LIGHTCYAN,
    LIGHTRED,
    PINK,
    YELLOW,
    WHITE,
};

typedef enum Color16 color16_t;

typedef struct 
{
	uint8_t character;
	uint8_t color;
} screen_cell_t;

typedef struct
{
	uint8_t x, y;
} cursor_t;

screen_cell_t* vga_getcell(uint8_t x, uint8_t y);

void vga_setfg(uint8_t x, uint8_t y, color16_t c);
void vga_setbg(uint8_t x, uint8_t y, color16_t c);
void vga_setcolors(uint8_t x, uint8_t y, color16_t fg, color16_t bg);

void vga_setch(uint8_t x, uint8_t y, char ch);

cursor_t vga_getcurr();
void vga_setcurr(uint8_t x, uint8_t y);
void vga_movcurr();

#endif //VGA_H
