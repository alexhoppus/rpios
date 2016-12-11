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
	        asm volatile("__delay_%=: subs %[count], %[count], #1; bne __delay_%=\n"
				                        : : [count]"r"(count) : "cc");
}
int main(int argc, char **argv)
{
	int a = 2, b = 2;
	delay(1000);
	mmio_write(AUX_MU_IO_REG, '*');
	while(1) {};
	return a + b;
}
