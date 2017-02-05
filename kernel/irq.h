#ifndef IRQ_H
#define IRQ_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "mmio.h"
#include "gpio.h"
#include "auxillary.h"

#define RPI2_IRQ_EN            (VPBASE + 0x0000B218)
#define RPI2_IRQ_TIMER_ENABLE  (1 << 0)

#define RPI2_IRQ_PENDING       (VPBASE + 0x0000B200)
#define RPI2_IRQ_TIMER_PENDING (1 << 0)

class rpi2_irqc_device {
public:
	rpi2_irqc_device();
	void init();
	uint32_t get_pending_irqs();
private:
	uint32_t base_irq_en;
	uint32_t base_irq_pending;
};
#endif
