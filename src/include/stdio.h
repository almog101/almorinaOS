#ifndef STDIO_H
#define STDIO_H

#define NULL (int64_t)0

#include <stdint.h>

void printf(const char* format, ...);
void putc(char c);
void puts(char *c);
void cls();

void fgets(char* dest, int n);
void set_fg_color(int color);

#endif
