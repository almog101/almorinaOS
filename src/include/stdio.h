#ifndef STDIO_H
#define STDIO_H

#include <stdint.h>

char getc();
void scanf(char* dst, int n);
void printf(const char* format, ...);
void putc(char c);
void puts(char *c);
void cls();

#endif
