#include "lib.h"

size_t strlen(const char* str)
{
	size_t ret = 0;
	while (str[ret] != 0)
		ret++;
	return ret;
};

int memcmp(const void *s1, const void *s2, size_t n)
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

int str_to_int(const char *str)
{
	int ret = 0;
	for (int i = 0; i < strlen(str); i++) {
		if ((str[i] < FIRST_NUM_ASCII) || (str[i] > LAST_NUM_ASCII))
			return -1;
		ret += (str[i] - FIRST_NUM_ASCII) * pow(DECIMAL_BASE, (strlen(str) - i - 1));
	}
	return ret;
}

void int_to_str(char *str, int val)
{
	int i = 0;
	int nval = val;
	bool is_neg = val < 0;
	while (nval) {
		nval = val / DECIMAL_BASE;
		int digit = val - nval * DECIMAL_BASE;
		str[i++] = (FIRST_NUM_ASCII + digit);
		val = nval;
	};
	if (is_neg)
		str[i++] = '-';
	str[i] = 0;
	string_reverse(str);
}
