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
	}while (*src != 0);
}

char *strchr(char *haystack, const char needle)
{
	while (*haystack != needle)
        if (!*haystack++)
            return 0;
    return haystack;
}
