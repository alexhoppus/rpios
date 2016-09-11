#ifndef UART_H
#define UART_H

#if !defined(__cplusplus)
#include <stdbool.h>
#endif
#include <stddef.h>
#include <stdint.h>

#include "../common/mmio.h"
#include "../common/gpio.h"
#include "../common/auxillary.h"


class rpi2_uart_device {
public:
	rpi2_uart_device();
        void putc(unsigned char c);
	unsigned char getc();
	void puts(const char* str);
	void arr_write(const unsigned char* buffer, size_t size);
	void init(int bit_fmt, int baudrate);
private:
	int bit_fmt;
	int baudrate;

	uint32_t aux_mu_io_reg;
	uint32_t aux_mu_ier_reg;
	uint32_t aux_mu_iir_reg;
	uint32_t aux_mu_lcr_reg;
	uint32_t aux_mu_mcr_reg;
	uint32_t aux_mu_lsr_reg;
	uint32_t aux_mu_msr_reg;
	uint32_t aux_mu_scratch;
	uint32_t aux_mu_cntl_reg;
	uint32_t aux_mu_stat_reg;
	uint32_t aux_mu_baud_reg;
};
#endif
