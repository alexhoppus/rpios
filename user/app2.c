#include <stddef.h>
#include <stdint.h>

#define AUX_MU_IO_REG 0xf2215040
static inline void mmio_write(uint32_t reg, uint32_t data)
{
	        *(volatile uint32_t *)reg = data;
}

static inline uint32_t mmio_read(uint32_t reg)
{
	        return *(volatile uint32_t *)reg;
}

/* Loop <delay> times in a way that the compiler won't optimize away. */
static inline void delay(int32_t count)
{
	asm volatile("mov r0, %[count]; __delay_%=: subs r0, r0, #1; bne __delay_%=\n"
			: : [count]"r"(count) : "cc");
}

extern "C" int puts(char *str);
extern "C" int yield(void);

int main(int argc, char **argv)
{
	while(1) {
		puts((char*)"Hey i'am app2!\n");
		delay(1000000);
	}
	return 0;
}
