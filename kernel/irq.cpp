#include "irq.h"
#include "kernel.h"
#include "exception.h"

rpi2_irqc_device::rpi2_irqc_device() : base_irq_en(RPI2_IRQ_EN),
	base_irq_pending(RPI2_IRQ_PENDING)
{
};

void rpi2_irqc_device::init()
{
	dprintk("irqc init\n");
	mmio_write(base_irq_en, RPI2_IRQ_TIMER_ENABLE);
}

uint32_t rpi2_irqc_device::get_pending_irqs()
{
	return mmio_read(base_irq_pending);
}

void irq_timer()
{
	sc.resched();
	timer.clear_irq();
}

extern "C" void irq_handler(struct trapframe *tf)
{
	uint32_t pend_irq = irqc.get_pending_irqs();
	dprintk("irq occured addr %lx spsr %lx\n", tf->pc, tf->spsr);
	/* only timer irq is supported now */
	irq_timer();
}


rpi2_irqc_device irqc;
