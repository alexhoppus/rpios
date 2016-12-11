#ifndef KERNEL_H
#define KERNEL_H
#include "../uart/uart.h"
#include "../mm/mm.h"
#include "../sched/sched.h"
#include "../mm/mmu.h"
#include "../console/console.h"
#include "auxillary.h"

/* Kernel global structure representation */

/* Low-level hardware interfaces */
/* Hardware devices drivers */
extern rpi2_uart_device uart;
/* Core kernel subsystems */
/* Console */
extern console *cons;
/* Memory manager */
extern memory_manager mm;

#endif
