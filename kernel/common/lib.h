#ifndef LIB_H
#define LIB_H

#if !defined(__cplusplus)
#include <stdbool.h>
#endif
#include <stddef.h>
#include <stdint.h>

size_t strlen(const char* str);
int memcmp(const void *s1, const void *s2, size_t n);
int str_to_int(const char *str);
int pow(int x, unsigned int y);
void int_to_str(char *str, int val);

#endif
