#include "console.h"
#include <stdarg.h>
#include "kernel.h"
#include "lib.h"

// Get an unsigned int of various possible sizes from a varargs list,
// depending on the lflag parameter.
static unsigned long long getuint(va_list *ap, int lflag)
{
	if (lflag >= 2)
		return va_arg(*ap, unsigned long long);
	else if (lflag)
		return va_arg(*ap, unsigned long);
	else
		return va_arg(*ap, unsigned int);
}

// Same as getuint but signed - can't use getuint
// because of sign extension
static long long getint(va_list *ap, int lflag)
{
	if (lflag >= 2)
		return va_arg(*ap, long long);
	else if (lflag)
		return va_arg(*ap, long);
	else
		return va_arg(*ap, int);
}

// TODO: Introduce normal div, moddiv and 64bit div
// TODO: This works incorrectly because of no 64 bit div support
void console::printnum(unsigned long long num, unsigned base)
{
	unsigned int digit_index = 0;
	// First recursively print all preceding (more significant) digits
	if (num >= base)
		printnum(div((uint32_t)num, (uint32_t) base), base);
	// then print this (the least significant) digit
	digit_index = num - div((uint32_t)num, base) * base;
	putc("0123456789abcdef"[digit_index]);
}

int console::cprintf(const char *fmt, ...)
{
	va_list ap;
	int cnt = 0;
	register int ch;
        unsigned long long num;
	int base, lflag = 0;
        register const char *p;
	va_start(ap, fmt);
	while (1) {
		while ((ch = *(unsigned char *) fmt++) != '%') {
			if (ch == '\0')
				return cnt;
			if (ch == '\n')
				putc('\r');
			putc(ch);
		}
		lflag = 0;
reswitch:
		switch (ch = *(unsigned char *) fmt++) {
			case 'l':
				lflag++;
				goto reswitch;
			case 'd':
				num = getint(&ap, lflag);
				if ((long long) num < 0) {
					putc('-');
					num = -(long long) num;
				}
				base = 10;
				goto number;
			case 'u':
				num = getuint(&ap, lflag);
				base = 10;
				goto number;
			case 'x':
				num = getuint(&ap, lflag);
				base = 16;
				putc('0');
				putc('x');
				goto number;
			case 's':
				if ((p = va_arg(ap, char *)) == NULL)
					p = "(null)";
				while ((ch = *p++) != '\0')
					putc(ch);
				break;
number:
				printnum(num, base);
				break;
		}
	}
	va_end(ap);
	return cnt;
}

console *cons;

