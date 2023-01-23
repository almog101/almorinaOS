#include "string.h"
#include "stdio.h"
#include <stdbool.h>

// Copy len bytes from src to dest.
void memcpy(uint8_t *dest, const uint8_t *src, uint32_t len)
{
    const uint8_t *sp = (const uint8_t *)src;
    uint8_t *dp = (uint8_t *)dest;
    for(; len != 0; len--) *dp++ = *sp++;
}

// Write len copies of val into dest.
void memset(uint8_t *dest, uint8_t val, uint32_t len)
{
    uint8_t *temp = (uint8_t *)dest;
    for ( ; len != 0; len--) *temp++ = val;
}

char *strcpy(char *dest, const char *src)
{
    do
	{
      	*dest++ = *src++;
	} while (*src != 0);
}

char *strncpy(char *dest, const char *src, int size)
{
	int copied_size = 0;
	do
	{
		copied_size++;
      	*dest++ = *src++;
	} while (*src != 0 && copied_size != size);
}

char *strchr(char *haystack, const char needle)
{
	while (*haystack != needle)
        if (!*haystack++)
            return 0;
    return haystack;
}

int strcmp(const char *str1, const char *str2)
{
	while (*str1)
	{
		if (*str1 != *str2)
			break;
		str1++;
		str2++;
	}
	return *(const unsigned char*)str1 - *(const unsigned char*)str2;
}

int atoi(char* str)
{
	int n=0;
	while (*(str))
		n = n*10 +  *(str++) - '0';
	return n;
}

int strlen(char* str)
{
	int n = 0;
	while (*(str))
	{
		n++;  
		str++;
	}
	return n;
}

int count(char* string, char ch)
{
	int n = 0;
	while (*(string))
	{
		if (*(string) == ch)
			n++;  
		string++;
	}
	return n;
}
