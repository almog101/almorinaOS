#include "stdio.h"
#include "stdarg.h"
#include "keycode.h"
#include "string.h"
#include <stdint.h>

// The VGA framebuffer starts at 0xB8000.
uint16_t *video_memory = (uint16_t *)0xB8000;
// Stores the cursor position.
static uint8_t cursor_x = 0;
static uint8_t cursor_y = 0;

static uint8_t bg_color = 1;
static uint8_t fg_color = 15;

// Updates the hardware cursor.
static void movcur()
{
    // The screen is 80 characters wide...
    uint16_t cursorLocation = cursor_y * 80 + cursor_x;
    outb(0x3D4, 14);                  // Tell the VGA board we are setting the high cursor byte.
    outb(0x3D5, cursorLocation >> 8); // Send the high cursor byte.
    outb(0x3D4, 15);                  // Tell the VGA board we are setting the low cursor byte.
    outb(0x3D5, cursorLocation);      // Send the low cursor byte.
}

// Scrolls the text on the screen up by one line.
static void scroll()
{

    // Get a space character with the default colour attributes.
    uint8_t attributeByte = (0 /*black*/ << 4) | (15 /*white*/ & 0x0F);
    uint16_t blank = 0x20 /* space */ | (attributeByte << 8);

    // Row 25 is the end, this means we need to scroll up
    if(cursor_y >= 25)
    {
        // Move the current text chunk that makes up the screen
        // back in the buffer by a line
        int i;
        for (i = 0*80; i < 24*80; i++)
        {
            video_memory[i] = video_memory[i+80];
        }

        // The last line should now be blank. Do this by writing
        // 80 spaces to it.
        for (i = 24*80; i < 25*80; i++)
        {
            video_memory[i] = blank;
        }
        // The cursor should now be on the last line.
        cursor_y = 24;
    }
}

// Writes a single character out to the screen.
void putc(char c)
{
    // The attribute byte is made up of two nibbles - the lower being the 
    // foreground colour, and the upper the background colour.
    uint8_t  attributeByte = (bg_color << 4) | (fg_color & 0x0F);
    // The attribute byte is the top 8 bits of the word we have to send to the
    // VGA board.
    uint16_t attribute = attributeByte << 8;
    uint16_t *location;

    // Handle a backspace, by moving the cursor back one space
    if (c == 0x08 && cursor_x)
    {
        cursor_x--;
    }

    // Handle a tab by increasing the cursor's X, but only to a point
    // where it is divisible by 8.
    else if (c == 0x09)
    {
        cursor_x = (cursor_x+8) & ~(8-1);
    }

    // Handle carriage return
    else if (c == '\r')
    {
        cursor_x = 0;
    }

    // Handle newline by moving cursor back to left and increasing the row
    else if (c == '\n')
    {
        cursor_x = 0;
        cursor_y++;
    }
    // Handle any other printable character.
    else if(c >= ' ')
    {
        location = video_memory + (cursor_y*80 + cursor_x);
        *location = c | attribute;
        cursor_x++;
    }

    // Check if we need to insert a new line because we have reached the end
    // of the screen.
    if (cursor_x >= 80)
    {
        cursor_x = 0;
        cursor_y ++;
    }

    // Scroll the screen if needed.
    scroll();
    // Move the hardware cursor.
    movcur();

}

void cls()
{
    // Make an attribute byte for the default colours
    uint8_t attributeByte = (bg_color << 4) | (fg_color & 0x0F);
    uint16_t blank = 0x20 /* space */ | (attributeByte << 8);

    for (int i = 0; i < 80*25; i++)
        video_memory[i] = blank;

    // Move the hardware cursor back to the start.
    cursor_x = 0;
    cursor_y = 0;
    movcur();
}

// Outputs a null-terminated ASCII string to the monitor.
void puts(char *c)
{
    int i = 0;
    while (c[i])
    {
        putc(c[i++]);
    }
}

void puth(uint32_t n)
{
    int tmp;

    puts("0x");

    char noZeroes = 1;

    int i;
    for (i = 28; i > 0; i -= 4)
    {
        tmp = (n >> i) & 0xF;
        if (tmp == 0 && noZeroes != 0)
        {
            continue;
        }
    
        if (tmp >= 0xA)
        {
            noZeroes = 0;
            putc (tmp-0xA+'a' );
        }
        else
        {
            noZeroes = 0;
            putc( tmp+'0' );
        }
    }
  
    tmp = n & 0xF;
    if (tmp >= 0xA)
    {
        putc (tmp-0xA+'a');
    }
    else
    {
        putc (tmp+'0');
    }

}

void putd(uint32_t n)
{
    if (n == 0)
    {
        putc('0');
        return;
    }

    int acc = n;
    char c[32];
    int i = 0;
    while (acc > 0)
    {
        c[i] = '0' + acc%10;
        acc /= 10;
        i++;
    }
    c[i] = 0;

    char c2[32];
    c2[i--] = 0;
    int j = 0;
    while(i >= 0)
    {
        c2[i--] = c[j++];
    }
    puts(c2);

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
	//cursor_x++;
	//movcur();
	va_end ( arguments );                  // Cleans up the list
}

static char last_ch = 0;
char getc()
{
	char ch;
	do
	{
		ch = inb(0x60);
	} while(ch == last_ch);

	last_ch = ch;
	return kbdmix[ch];
}

void scanf(char* dst, int n)
{
	memset(dst, 0, n);
	int i = 0;
	while (i < n)
	{
		char ch = getc();
		if (ch)
		{
			putc(ch);
			dst[i++] = ch;
			if (ch == '\n')
				break;
		}
	}
}


