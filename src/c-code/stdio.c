#include "stdio.h"
#include <stdarg.h>
#include "string.h"
#include "vga.h"
#include <stdint.h>
#include <stdbool.h>
#include "keyboard.h"

static color16_t bg_color = BLUE;
static color16_t fg_color = WHITE;

void scroll(cursor_t* curr)
{
	if (curr->y >= SCREEN_HEIGHT)
	{
		// move each line 1 row up
		for (int i = 0; i<SCREEN_HEIGHT-1; i++)
			for (int j = 0; j<SCREEN_WIDTH; j++)
				vga_setch(j, i, vga_getch(j, i+1));
		
		// clear the last line
		for (int j = 0; j<SCREEN_WIDTH; j++) 
			vga_setch(j, SCREEN_HEIGHT-1, BLANK_CHAR);

		curr->y = SCREEN_HEIGHT-1;
		curr->x =0;
	}
}

void putc(char c)
{
	cursor_t curr = vga_getcurr();

    if (c == BACKSPACE_CHAR && curr.x)
	{
        curr.x--;
		vga_setcell(curr.x, curr.y, ' ', fg_color, bg_color);
	}
    else if (c == TAB_CHAR)
        curr.x = (curr.x+8) & ~(8-1); // increases the cursor's X, but only if it's divisible by 8
    else if (c == RETURN_CHAR)
        curr.x = 0;
    else if (c == NEWLINE_CHAR)
    {
        curr.x = 0;
        curr.y++;
    }
    else if(c >= BLANK_CHAR) // Handle any other printable character.
    {
		vga_setcell(curr.x, curr.y, c, fg_color, bg_color);
        curr.x++;
    }

    if (curr.x >= SCREEN_WIDTH) // if reached the end of the line, add new line
    {
        curr.x = 0;
        curr.y++;
    }

	scroll(&curr);
	vga_setcurr(curr.x, curr.y);
	vga_movcurr();
}

void cls()
{
    for (int i = 0; i < 25; i++)
		for (int j = 0; j < 80; j++)
			vga_setcell(j, i, BLANK_CHAR, fg_color, bg_color);

	vga_setcurr(0, 0);
	vga_movcurr();
}

void puts(char *c)
{
    int i = 0;
    while (c[i])
        putc(c[i++]);
}

void puth(int n, bool uppercase)
{
    puts("0x");
    
	const int BYTE_SIZE = 8;
	const int DIGIT_BITS = 4;

	int digit;
    bool noZeroes = true;

    for (int i = sizeof(n) * BYTE_SIZE - DIGIT_BITS; i >= 0; i -= DIGIT_BITS)
    {
        digit = (n >> i) & 0xF;
        
		if (digit == 0 && noZeroes != 0)
            continue;
		noZeroes = 0;

        if (digit >= 0xA)
            putc ((digit-0xA) + ((uppercase) ? 'A' : 'a') );
        else
            putc( digit+'0' );
    }
}

void putd(int n)
{ 
	if( n > 9 )
	{ 
		int a = n / 10;
		n -= 10 * a;
		putd(a);
	}
	putc('0'+n);
}

void printf(const char* format, ...)
{
	va_list arguments;                     
	va_start ( arguments, format );   

	for (int i =0; format[i]; i++)
	{
		if (format[i] == '%')
		{
			switch (format[i+1])
			{
				case 'd':
					putd(va_arg ( arguments, int ));
					break;
				case 'x':
					puth(va_arg ( arguments, int ), false);
					break;
				case 'X':
					puth(va_arg ( arguments, int ), true);
					break;
				case 'c':
					putc(va_arg ( arguments, char ));
					break;
				case 's':
					puts(va_arg ( arguments, char* ));
					break;
				default:
					break;
			}
			i++;
		}
		else
			putc(format[i]);
	}

	va_end ( arguments );                  // Cleans up the list
}

void fgets(char* dest, int n)
{
	char temp = keyboard_getch(), ch =temp;
	int i =0;
	do
	{
		ch = keyboard_getch();
		if (ch != temp && ch)
		{
			temp = ch;
			dest[i++] = ch;
		}
	} while(i<n && ch != NEWLINE_CHAR);
}
