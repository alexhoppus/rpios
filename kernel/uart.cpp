#include "uart.h"
#include "lib.h"
#include "system.h"
#include "gpio.h"

rpi2_uart_device::rpi2_uart_device() : aux_mu_io_reg(AUX_MU_IO_REG), aux_mu_ier_reg(AUX_MU_IER_REG),
	aux_mu_iir_reg(AUX_MU_IIR_REG), aux_mu_lcr_reg(AUX_MU_LCR_REG), aux_mu_mcr_reg(AUX_MU_MCR_REG),
	aux_mu_lsr_reg(AUX_MU_LSR_REG), aux_mu_msr_reg(AUX_MU_MSR_REG), aux_mu_scratch(AUX_MU_SCRATCH),
	aux_mu_cntl_reg(AUX_MU_CNTL_REG), aux_mu_stat_reg(AUX_MU_STAT_REG), aux_mu_baud_reg(AUX_MU_BAUD_REG)
{
};

void rpi2_uart_device::init()
{
	int bit_fmt = 8, baudrate = 115200;
	uint32_t aux_mu_lcr_reg_val = 0, aux_mu_iir_reg_val = 0,
		 aux_mu_baud_reg_val = 0, aux_mu_cntl_reg_val = 0;
	/*
	 * Enable auxillary devices,
	 * actually we care about mini uart (MU)
	 * */
	mmio_write(AUX_ENABLES, 1);

	mmio_write(aux_mu_ier_reg, 0);
	mmio_write(aux_mu_cntl_reg, 0);
	mmio_write(aux_mu_mcr_reg, 0);

	(bit_fmt == 8) ? aux_mu_lcr_reg_val |= AUX_MU_LCR_REG_8BITMODE : 1;
	aux_mu_iir_reg_val |= (AUX_MU_IIR_REG_CLEAR_PENDINT | AUX_MU_IIR_REG_CLEAR_RFIFO |
			AUX_MU_IIR_REG_CLEAR_WFIFO);
	aux_mu_baud_reg_val = div(SYSTEM_CLOCK_HZ, baudrate * 8) - 1;
	aux_mu_cntl_reg_val = (AUX_MU_CNTL_REG_RE | AUX_MU_CNTL_REG_TE);
	/*
	 * Can't figure out what the hell is this.
	 * From datasheet it is seen that lcr 1'st bit should be
	 * 1 for 8 bit mode, and 0 for 7 bit mode. In all tutorials
	 * 0x3 is written to UART, and it really prints the trash
	 * without this setup.
	 * */
	mmio_write(aux_mu_lcr_reg, aux_mu_lcr_reg_val);
	mmio_write(aux_mu_iir_reg, aux_mu_iir_reg_val);
	mmio_write(aux_mu_baud_reg, aux_mu_baud_reg_val);
	/*GPIO 15 and 14 have alt function 5*/
	gpio_set_function(GPFSEL1, 4, GPIO_F5);
	gpio_set_function(GPFSEL1, 5, GPIO_F5);

	mmio_write(GPPUD, 0);
	delay(150);
	mmio_write(GPPUDCLK0, (1 << 14) | (1 << 15));
	delay(150);
	mmio_write(GPPUDCLK0, 0);
	/*Enable transmitter and reciever*/
	mmio_write(aux_mu_cntl_reg, aux_mu_cntl_reg_val);
}

void rpi2_uart_device::putc(unsigned char c)
{
	while (true)
		if (mmio_read(aux_mu_lsr_reg) & AUX_MU_LST_REG_TRDY)
			break;
	mmio_write(aux_mu_io_reg, c);
}

unsigned char rpi2_uart_device::getc()
{
	while (true)
		if (mmio_read(aux_mu_lsr_reg) & AUX_MU_LST_REG_RRDY)
			break;
	return(mmio_read(aux_mu_io_reg) & 0xFF);
}

void rpi2_uart_device::arr_write(const unsigned char* buffer, size_t size)
{
	for (size_t i = 0; i < size; i++ ) {
		if (buffer[i] == '\n')
			putc('\r');
		putc(buffer[i]);
	}
}

void rpi2_uart_device::puts(const char* str)
{
	arr_write((const unsigned char*) str, strlen(str));
}

rpi2_uart_device uart;
