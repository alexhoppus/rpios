#ifndef KERNEL_H
#define KERNEL_H
#include "uart.h"
#include "timer.h"
#include "irq.h"
#include "mm.h"
#include "sched.h"
#include "mmu.h"
#include "console.h"
#include "auxillary.h"

/* Kernel global structure representation */

/* Low-level hardware interfaces */
/* Hardware devices drivers */
extern rpi2_uart_device uart;
extern rpi2_timer_device timer;
extern rpi2_irqc_device irqc;

/* Core kernel subsystems */
/* Console */
extern console *cons;
/* Memory manager */
extern memory_manager mm;
/* Scheduler */
extern scheduler sc;
#endif
