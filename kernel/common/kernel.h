#ifndef KERNEL_H
#define KERNEL_H
#include "../uart/uart.h"
#include "../mm/mm.h"
#include "../mm/mmu.h"
#include "../console/console.h"
#include "auxillary.h"

/* Kernel global structure representation */

class kernel {
private:
public:
	/* Low-level hardware interfaces */
	/* Hardware devices drivers */
	rpi2_uart_device uart;
	/* MMU device */
	mmu_device mmu;
	/* Core kernel subsystems */
	/* Console */
	console *cons;
	/* Memory manager */
	memory_manager mm;
};
extern kernel kern;
#endif
