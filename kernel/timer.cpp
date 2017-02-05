#include "timer.h"
#include "kernel.h"

rpi2_timer_device::rpi2_timer_device() : load(RPI2_TIMER_LOAD_ADDR),
	control(RPI2_TIMER_CTRL_ADDR), irq_clear(RPI2_TIMER_CLEAR_ADDR)
{
};

void rpi2_timer_device::init()
{
	dprintk("timer init\n");
	mmio_write(load, RPI2_TIMER_LOAD_VALUE);
	mmio_write(control, RPI2_TIMER_23BIT_CNT |
		RPI2_TIMER_PRESCALE_256 | RPI2_TIMER_IRQ_ENABLE | RPI2_TIMER_ENABLE);

}

void rpi2_timer_device::clear_irq()
{
	 mmio_write(irq_clear, RPI2_CLEAR_TIMER_IRQ);
}
rpi2_timer_device timer;
