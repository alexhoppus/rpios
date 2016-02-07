#ifndef UART_H
#define UART_H

#if !defined(__cplusplus)
#include <stdbool.h>
#endif
#include <stddef.h>
#include <stdint.h>

#include "../common/mmio.h"
#include "../common/gpio.h"

/* Mini UART 1 base ports information*/
#define AUX_ENABLES     (PBASE + 0x00215004)
/* Mini Uart I/O Data */
#define AUX_MU_IO_REG   (PBASE + 0x00215040)
/* Mini Uart Interrupt Enable */
#define AUX_MU_IER_REG  (PBASE + 0x00215044)
/* Mini Uart Interrupt Identifity */
#define AUX_MU_IIR_REG  (PBASE + 0x00215048)
/* Mini Uart Line Control */
#define AUX_MU_LCR_REG  (PBASE + 0x0021504C)
/* Mini Uart Modem Control */
#define AUX_MU_MCR_REG  (PBASE + 0x00215050)
/* Mini Uart Line Status */
#define AUX_MU_LSR_REG  (PBASE + 0x00215054)
/* Mini Uart Modem Status */
#define AUX_MU_MSR_REG  (PBASE + 0x00215058)
/* Mini Uart Scratch */
#define AUX_MU_SCRATCH  (PBASE + 0x0021505C)
/* Mini Uart Extra Control */
#define AUX_MU_CNTL_REG (PBASE + 0x00215060)
/* Mini Uart Extra Status */
#define AUX_MU_STAT_REG (PBASE + 0x00215064)
/* Mini Uart Baudrate */
#define AUX_MU_BAUD_REG (PBASE + 0x00215068)

class rpi2_uart_device {
public:
	rpi2_uart_device();
        void putc(unsigned char c);
	unsigned char getc();
	void puts(const char* str);
private:
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

	void str_write(const unsigned char* buffer, size_t size);
	void init();
};
#endif
