#include "uart/uart.h"
#include "common/stubs.h"
#include "common/lib.h"

#define BOOTLOADER_MAGIC "blabla"
#define BOOTLOADER_MAGIC_LEN 6
char magic[BOOTLOADER_MAGIC_LEN] = {0};
char image_size[30] = {0};
char exception_str[30] = {0};
#define BOOT_ADDRESS 0x32000
#define LINE_FEED 0x0A

#define APPROX_CLK 800*1024*1024
#define BAUD_RATE 115200
#define UART_FIFO_SIZE 16
#define DELAY_UART_FLUSH UART_FIFO_SIZE * 8 * (APPROX_CLK / BAUD_RATE)

rpi2_uart_device *guart;

#if defined(__cplusplus)
extern "C"
#endif
void exception_handler(uint32_t r0)
{
	uint32_t link_register = 0;
	guart->puts("Exception handler, code: ");
	guart->puts(int_to_str(exception_str, r0, 16));
	guart->puts("\n");
	guart->puts("Link register: ");
	asm volatile("mov %[r], lr": [r]"=r" (link_register)::);
	guart->puts(int_to_str(exception_str, link_register, 16));
	guart->puts("\n");
	while(1){};
}

void uart_load_kernel(rpi2_uart_device *uart)
{
	int i = 0;
	while ((image_size[i++] = uart->getc()) != LINE_FEED);
	uart->puts("Image size (bytes): ");
	uart->puts(image_size);
	image_size[i - 1] = 0;
	{
		int img_size = str_to_int(image_size);
		char *kernel = (char *) BOOT_ADDRESS;
		for (int i = 0; i < img_size; i++)
			kernel[i] = uart->getc();
		uart->puts("\n");
		uart->puts("Data transfer finished, giving up control to kernel\n");
		/* Waiting for all uart stuff to be flushed */
		delay(DELAY_UART_FLUSH);
		register uint32_t r0 asm("r0") = (uint32_t) kernel;
		asm volatile("bx r0");
	}
}

#if defined(__cplusplus)
extern "C"
#endif
void bootloader_main(void)
{
	unsigned long try_ctr = 0;
	const int bit_format = 8;
	const int baudrate = 115200;
	rpi2_uart_device uart;
	guart = &uart;
	uart.init(bit_format, baudrate);
	uart.puts("Waiting for image data on UART\n");
	while(true) {
		if (!memcmp(magic, BOOTLOADER_MAGIC, (size_t) BOOTLOADER_MAGIC_LEN)) {
			uart.puts("Data transfer detected, downloading the image ...\n");
			uart_load_kernel(&uart);
		}
		magic[try_ctr++ % BOOTLOADER_MAGIC_LEN] = uart.getc();
	};
}
