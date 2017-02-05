#ifndef TIMER_H
#define TIMER_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "mmio.h"
#include "gpio.h"
#include "auxillary.h"

#define RPI2_TIMER_LOAD_ADDR     (VPBASE + 0x0000B400)
#define RPI2_TIMER_CTRL_ADDR     (VPBASE + 0x0000B408)
#define RPI2_TIMER_CLEAR_ADDR    (VPBASE + 0x0000B40C)

#define RPI2_TIMER_LOAD_VALUE    0x400
#define RPI2_TIMER_23BIT_CNT     (1 << 1)
#define RPI2_TIMER_PRESCALE_256  (0b10 << 2)
#define RPI2_TIMER_IRQ_ENABLE    (1 << 5)
#define RPI2_TIMER_ENABLE        (1 << 7)
#define RPI2_CLEAR_TIMER_IRQ     (1 << 0)

class rpi2_timer_device {
public:
	rpi2_timer_device();
	void init();
	void clear_irq();
private:
	uint32_t base_irq_addr; /*Should be owner by IRQ subsystem */
	uint32_t load;
	uint32_t control;
	uint32_t irq_clear;
};
#endif
