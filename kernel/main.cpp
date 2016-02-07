#include "uart/uart.h"
#include "common/stubs.h"
#include "common/lib.h"

#if defined(__cplusplus)
extern "C"
#endif
void kernel_main(uint32_t r0, uint32_t r1, uint32_t atags)
{
	(void) r0;
	(void) r1;
	(void) atags;
	unsigned long try_ctr = 0;
	rpi2_uart_device uart;
	uart.puts("Hi iam kernel\n");
}
