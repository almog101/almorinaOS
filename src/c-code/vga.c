#include "vga.h"

screen_cell_t* video_memory = (screen_cell_t*)0xB8000;
cursor_t cursor = {0,0};

screen_cell_t* vga_getcell(uint8_t x, uint8_t y)
{
	return video_memory + y*SCREEN_WIDTH + x;
}

void vga_setch(uint8_t x, uint8_t y, char ch)
{
	vga_getcell(x,y)->character = ch;
}

char vga_getch(uint8_t x, uint8_t y)
{
	return vga_getcell(x,y)->character;
}

void vga_setfg(uint8_t x, uint8_t y, color16_t c)
{
	screen_cell_t* cell = vga_getcell(x,y);
	uint16_t bg_color = cell->color & 0xF0;
	cell->color = bg_color | c;
}

void vga_setbg(uint8_t x, uint8_t y, color16_t c)
{
	screen_cell_t* cell = vga_getcell(x,y);
	uint16_t fg_color = cell->color & 0x0F;
	cell->color = c<<4 | fg_color;
}

void vga_setcolors(uint8_t x, uint8_t y, color16_t fg, color16_t bg)
{
	vga_getcell(x,y)->color = (uint8_t)bg<<4 | (uint8_t)fg;
}


void vga_setcell(uint8_t x, uint8_t y, char ch, color16_t fg, color16_t bg)
{
	vga_setch(x,y,ch);
	vga_setcolors(x,y, fg,bg);
}

cursor_t vga_getcurr()
{
	return cursor;
}

void vga_setcurr(uint8_t x, uint8_t y)
{
	cursor.x = x;
	cursor.y =y;
}

void vga_movcurr()
{
    uint16_t currpos = cursor.y * 80 + cursor.x;
    outb(0x3D4, 14);                // Tell the VGA board we are setting the high cursor byte.
    outb(0x3D5, currpos & 0xF0); 	// Send the high cursor byte.
    outb(0x3D4, 15);                // Tell the VGA board we are setting the low cursor byte.
    outb(0x3D5, currpos & 0x0F);    // Send the low cursor byte.
}
