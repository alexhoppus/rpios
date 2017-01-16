#include "lib.h"
#include "../common/kernel.h"

extern "C" void memset(void *b, const unsigned char c, int len)
{
	unsigned char *p = (unsigned char *)b;
	while (len > 0) {
		*p = c;
		p++;
		len--;
	}
}

extern "C" void *memmove(void *dst, const void *src, size_t n)
{
	const char *s;
	char *d;

	s = (char *)src;
	d = (char *)dst;
	if (s < d && s + n > d) {
		s += n;
		d += n;
		while (n-- > 0)
			*--d = *--s;
	} else
		while (n-- > 0)
			*d++ = *s++;

	return dst;
}

extern "C" void *memcpy(void *dst, const void *src, size_t n)
{
	return memmove(dst, src, n);
}

size_t strlen(const char* str)
{
	size_t ret = 0;
	while (str[ret] != 0)
		ret++;
	return ret;
};

extern "C" int memcmp(const void *s1, const void *s2, size_t n)
{
	register const char *cs1 = (char *)s1;
	register const char *cs2 = (char *)s2;
	for (size_t i = 0; i < n; i++) {
		if (cs1[i] != cs2[i])
			return cs1[i] - cs2[i];
	}
	return 0;
}

int pow(int x, unsigned int y)
{
	if( y == 0)
		return 1;
	int t = pow(x, y / 2);
	if (y % 2 == 0)
		return t * t;
	else
		return x * t * t;
}

char *string_reverse(char *str)
{
	if (!*str)
		return (str);

	char *p1;
	char *p2;
	for (p1 = str, p2 = str + strlen(str) - 1; p2 > p1; ++p1, --p2) {
		*p1 ^= *p2;
		*p2 ^= *p1;
		*p1 ^= *p2;
	}
	return str;
}
#define FIRST_NUM_ASCII '0'
#define LAST_NUM_ASCII '9'
#define DECIMAL_BASE 10

/*TODO: Implement common converter*/
int str_to_int(const char *str)
{
	int ret = 0;
	for (int i = 0; i < (int)strlen(str); i++) {
		if ((str[i] < FIRST_NUM_ASCII) || (str[i] > LAST_NUM_ASCII))
			return -1;
		ret += (str[i] - FIRST_NUM_ASCII) * pow(DECIMAL_BASE, (strlen(str) - i - 1));
	}
	return ret;
}

char* int_to_str(char *str, unsigned long val, int base)
{
	int i = 0;
	int nval = val;
	char ascii[] = {"0123456789ABCDEF"};

	if (base != 10 && base != 16)
		return NULL;

	while (nval) {
		nval = div(val, base);
		int digit = val - nval * base;
		str[i++] = ascii[digit];
		val = nval;
	};
	str[i] = 0;
	string_reverse(str);
	return str;
}

/*TODO: That is not good, implement aeabi_udiv3 natively in asm */
uint32_t div(uint32_t n, uint32_t d)
{
	uint32_t q = 0, r = 0;
	int i;
	if (d == 1)
		return n;
	if (d == 0)
		panic("Zero division\n");

	if (is_pow_of_two(d)) {
		int shift = 0;
		while (d >>= 1) {shift++;};
		return n >> shift;
	}

	for(i = 31; i >= 0; i--){
		r = r << 1;
		r = r | ((n >> i) & 1);
		if(r >= d) {
			r = r - d;
			q = q | (1 << i);
		}
	}
	return q;
}
