#ifndef CONS_H
#define CONS_H
class console {
private:
	void printnum(unsigned long long num, unsigned base);
public:
	console() {};
	void init();
	void putchar(unsigned char ch);
	int cprintf(const char *fmt, ...);
};
#endif
