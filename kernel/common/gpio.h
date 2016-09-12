#ifndef GPIO_H
#define GPIO_H
#include "../common/mmio.h"

#define GPFSEL1         (VPBASE + 0x00200004)
#define GPSET0          (VPBASE + 0x0020001C)
#define GPCLR0          (VPBASE + 0x00200028)
#define GPPUD           (VPBASE + 0x00200094)
#define GPPUDCLK0       (VPBASE + 0x00200098)

/*GPFSEL{1..n}* registers handling GPIO pins
 * functions. F.e. GPFSEL0 handle pins 0 - 9
 * GPFSEL1 10 - 19.
 *
 * GPIO_FUNCTION_MASK
 * GPFSELn - Function Select n
 * 000 = GPIO Pin n is an input
 * 001 = GPIO Pin n is an output
 * 100 = GPIO Pin n takes alternate function 0
 * 101 = GPIO Pin n takes alternate function 1
 * 110 = GPIO Pin n takes alternate function 2
 * 111 = GPIO Pin n takes alternate function 3
 * 011 = GPIO Pin n takes alternate function 4
 * 010 = GPIO Pin n takes alternate function 5
 *
 * For alternative function list reference Broadcom manual
 * for 2835 soc p. 102
 */
#define GPIO_IN 0x0
#define GPIO_OUT 0x1
#define GPIO_F0 0x4
#define GPIO_F1 0x5
#define GPIO_F2 0x6
#define GPIO_F3 0x7
#define GPIO_F4 0x3
#define GPIO_F5 0x2

#define GPIO_FUNCTION_MASK 0x7

/*TODO: Think about convinient interface for setting GPIO functions*/

static inline void gpio_set_function(uint32_t gpf_reg, int gpio_wire_offset, int gpio_func)
{
	uint32_t gpf_reg_val = mmio_read(gpf_reg);
	gpf_reg_val &= ~(GPIO_FUNCTION_MASK << (gpio_wire_offset * 3));
	gpf_reg_val |= gpio_func << (gpio_wire_offset * 3);
	mmio_write(gpf_reg, gpf_reg_val);
}

#endif
