#include "string.h"
#include "stdio.h"
#include <stdbool.h>

/// Copy len bytes from src to dest.
/// INPUT: [string to copy to], [string to copy from], [number of bytes to copy]
/// OUTPUT: none
void memcpy(uint8_t *dest, const uint8_t *src, uint32_t len)
{
    const uint8_t *sp = (const uint8_t *)src;
    uint8_t *dp = (uint8_t *)dest;
    for(; len != 0; len--) *dp++ = *sp++;
}

/// Write len copies of val into dest.
/// INPUT: [string to copy to], [char to copy], [number of times to copy]
/// OUTPUT: none
void memset(uint8_t *dest, uint8_t val, uint32_t len)
{
    uint8_t *temp = (uint8_t *)dest;
    for (; len != 0; len--) 
		*temp++ = val;
}

/// Copy src to dest.
/// INPUT: [string to copy to], [string to copy from]
/// OUTPUT: the copied string
char* strcpy(char *dest, const char *src)
{
    char *temp = dest;
    while(*dest++ = *src++);
    return temp;
}

/// Copy src to dest size bytes.
/// INPUT: [string to copy to], [string to copy from], [number of butes to copy]
/// OUTPUT: the copied string
char* strncpy(char *dest, const char *src, int size)
{
	char* temp = dest;
	do
	{
		size--;
      	*dest++ = *src++;
	} while (*src != 0 && size);
	return temp;
}

/// Get the location of needle in haystack.
/// INPUT: [string to search in], [char to find]
/// OUTPUT: needle's location (in haystack)
char* strchr(char *haystack, const char needle)
{
	while (*haystack != needle)
	{
        if (!*haystack++)
            return 0;
	}
    return haystack;
}

/// Compare str1 to str2.
/// INPUT: [first string], [second string]
/// OUTPUT: result of the comparison [0 if they are the same]
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

/// Convert str to int.
/// INPUT: [string with numberic chars]
/// OUTPUT: number of str
int atoi(char* str)
{
	int n = 0;
	while (*(str))
		n = n*10 +  *(str++) - '0';
	return n;
}

/// Convert str to float.
/// INPUT: [string with numberic chars]
/// OUTPUT: float number
double atof(const char *s)
{
  double a = 0.0;
  int e = 0;
  int c;
  while ((c = *s++) != '\0' && isdigit(c)) {
    a = a*10.0 + (c - '0');
  }
  if (c == '.') {
    while ((c = *s++) != '\0' && isdigit(c)) {
      a = a*10.0 + (c - '0');
      e = e-1;
    }
  }
  if (c == 'e' || c == 'E') {
    int sign = 1;
    int i = 0;
    c = *s++;
    if (c == '+')
      c = *s++;
    else if (c == '-') {
      c = *s++;
      sign = -1;
    }
    while (isdigit(c)) {
      i = i*10 + (c - '0');
      c = *s++;
    }
    e += i*sign;
  }
  while (e > 0) {
    a *= 10.0;
    e--;
  }
  while (e < 0) {
    a *= 0.1;
    e++;
  }
  return a;
}

/// Get str length.
/// INPUT: [a string]
/// OUTPUT: length of str
int strlen(const char* str)
{
	int n = 0;
	while (*(str++))
	{
		n++;
	}
	return n;
}

/// Get who many time ch appeared in str.
/// INPUT: [string to check], [char to search]
/// OUTPUT: number of times
int count(char* str, char ch)
{
	int n = 0;
	while (*(str))
	{
		if (*(str++) == ch)
			n++; 
	}
	return n;
}
