#ifndef STRING_H
#define STRING_H

#include <stdint.h>

void memcpy(uint8_t *dest, const uint8_t *src, uint32_t len);
void memset(uint8_t *dest, uint8_t val, uint32_t len);

char *strcpy(char *dest, const char *src);
char *strncpy(char *dest, const char *src, int size);
char *strchr(char *haystack, const char needle);
int strcmp(const char *str1, const char *str2);
int atoi(char* str);
int strlen(const char* str);
int count(char* string, char ch);

#endif // STRING_H

