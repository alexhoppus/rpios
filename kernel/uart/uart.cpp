#include "uart.h"
#include "../common/lib.h"

rpi2_uart_device::rpi2_uart_device() : aux_mu_io_reg(AUX_MU_IO_REG), aux_mu_ier_reg(AUX_MU_IER_REG),
	aux_mu_iir_reg(AUX_MU_IIR_REG), aux_mu_lcr_reg(AUX_MU_LCR_REG), aux_mu_mcr_reg(AUX_MU_MCR_REG),
	aux_mu_lsr_reg(AUX_MU_LSR_REG), aux_mu_msr_reg(AUX_MU_MSR_REG), aux_mu_scratch(AUX_MU_SCRATCH),
	aux_mu_cntl_reg(AUX_MU_CNTL_REG), aux_mu_stat_reg(AUX_MU_STAT_REG), aux_mu_baud_reg(AUX_MU_BAUD_REG)
{
	init();
};

void rpi2_uart_device::init()
{
	unsigned int ra;

	mmio_write(AUX_ENABLES, 1);
	mmio_write(aux_mu_ier_reg, 0);
	mmio_write(aux_mu_cntl_reg, 0);
	mmio_write(aux_mu_lcr_reg, 3);
	mmio_write(aux_mu_mcr_reg, 0);
	mmio_write(aux_mu_ier_reg, 0);
	mmio_write(aux_mu_iir_reg, 0xC6);
	mmio_write(aux_mu_baud_reg, 270);
	ra = mmio_read(GPFSEL1);
	ra &= ~(7 << 12); //gpio14
	ra |= 2 << 12;    //alt5
	ra &= ~(7 << 15); //gpio15
	ra |= 2 << 15;    //alt5
	mmio_write(GPFSEL1, ra);
	mmio_write(GPPUD, 0);
	delay(150);
	mmio_write(GPPUDCLK0, (1 << 14) | (1 << 15));
	delay(150);
	mmio_write(GPPUDCLK0, 0);
	mmio_write(aux_mu_cntl_reg, 3);

}

void rpi2_uart_device::putc(unsigned char c)
{
	while(1)
		if (mmio_read(aux_mu_lsr_reg) & 0x20)
			break;
	mmio_write(aux_mu_io_reg, c);
}

unsigned char rpi2_uart_device::getc()
{
	while(1)
		if (mmio_read(aux_mu_lsr_reg) & 0x01)
			break;
	return(mmio_read(aux_mu_io_reg) & 0xFF);
}

void rpi2_uart_device::str_write(const unsigned char* buffer, size_t size)
{
	for (size_t i = 0; i < size; i++ ) {
		if (buffer[i] == '\n')
			putc('\r');
		putc(buffer[i]);
	}
}

void rpi2_uart_device::puts(const char* str)
{
	str_write((const unsigned char*) str, strlen(str));
}
